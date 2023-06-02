/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <Log.h>
#include <utils/EnvironmentUtils.h>
#include <utils/LocaleUtils.h>
#include <utils/StrUtils.h>
#include <utils/json-cpp.h>

#include <filesystem>
#include <fstream>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace HCXX {

namespace StrLocales {

std::shared_mutex g_lock;

class LocalizationsProvider {
public:
    static LocalizationsProvider& Instance()
    {
        static LocalizationsProvider instance;
        return instance;
    }

    void Load(std::ifstream& jsonInput, const std::string_view requestedLocale)
    {
        std::string locale = StrUtils::ToUpper(requestedLocale);
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

        try {
            jsoncpp::parse(data, jsonInput, true);

        }
        // Malformed JSON file is received as input. Just report it and continue processing
        // LCOV_EXCL_START
        catch (std::exception& ex) {
            HCXX::Log(HCXX::LogLevel::ERROR) << "Failed to parse localization file: " << ex.what() << "\n";
            return;
        }
        // LCOV_EXCL_STOP

        myStrings.reserve(myStrings.size() + data.size());
        for (auto& item : data) {
            auto insertResult = myKeys.insert(item.first);
            if (!insertResult.second) {
                // This case can happen if localized strings JSON files contain duplicated entries
                // It is project configuration issue and have to be fixed by introducing new string ID or removing
                // duplicates
                // LCOV_EXCL_START
                HCXX::Log(HCXX::LogLevel::WARNING) << "String for ID '" << item.first << "' already added. Skipping\n";
                continue;
                // LCOV_EXCL_STOP
            }

            std::string_view key = *(insertResult.first);
            auto str = item.second.find(locale) != item.second.end() ? item.second[locale]
                       : item.second.find(myDefaultLocale) != item.second.end()
                           ? item.second[myDefaultLocale]
                           : myUnknownString + "'" + item.first + "'";

            HCXX::Log(HCXX::LogLevel::DEBUG) << "Loading string for ID '" << item.first << "' = '" << str << "'\n";
            myStrings.emplace(key, str);
        }
    }

    std::string Get(std::string_view& id, const bool hideWarning) const
    {
        auto lookupId = myStrings.find(id);
        if (lookupId == myStrings.end()) {
            if (!hideWarning) {
                HCXX::Log(HCXX::LogLevel::WARNING) << "String for ID '" << id << "' is not found. "
                                                   << "Number of strings " << myStrings.size() << "\n";
            }
            return "";
        } else {
            return lookupId->second;
        }
    }

    std::size_t GetCount()
    {
        return myStrings.size();
    }

    // This method is only for testing purposes and not planned to be used in production
    void Reset()
    {
        myStrings.clear();
        myKeys.clear();
    }

private:
    // Singleton requirements:
    LocalizationsProvider() = default;
    LocalizationsProvider(const LocalizationsProvider&) = delete;
    LocalizationsProvider& operator=(const LocalizationsProvider&) = delete;

    using KeySet = std::unordered_set<std::string>;
    using StringsMap = std::unordered_map<std::string_view, std::string>;

    const char myDefaultLocale[3] = "EN";
    const std::string myUnknownString{"#Cooddy_Unknown_String "};

    KeySet myKeys;
    StringsMap myStrings;
};

std::string GetStringLocale(std::string_view id, const std::vector<std::string>& replacements, const bool hideWarning)
{
    std::shared_lock<std::shared_mutex> lock(g_lock);

    auto localizedString = LocalizationsProvider::Instance().Get(id, hideWarning);

    if (localizedString.empty()) {
        return "";
    }

    if (replacements.empty()) {
        return localizedString;
    }

    return StrUtils::ReplacePlaceholders(localizedString, replacements);
}

bool LoadLocale(const std::filesystem::path& localeFilePath, std::string_view locale)
{
    HCXX::Log(HCXX::LogLevel::DEBUG) << "Loading localization file '" << localeFilePath << "' for locale '" << locale
                                     << "'\n";
    std::ifstream localeFile(localeFilePath);

    if (!localeFile.is_open()) {
        // LCOV_EXCL_START
        HCXX::Log(HCXX::LogLevel::FATAL) << "Could not open localization file '" << localeFilePath << "'\n";
        return false;
        // LCOV_EXCL_STOP
    }

    LocalizationsProvider::Instance().Load(localeFile, StrUtils::ToLower(locale));
    HCXX::Log(HCXX::LogLevel::DEBUG) << "Loaded " << LocalizationsProvider::Instance().GetCount() << " strings\n";
    return true;
}

bool LoadLocale(std::string_view locale)
{
    bool ret = true;
    std::filesystem::path cooddyConfigRoot = EnvironmentUtils::GetSelfExecutableDir() / ".cooddy";

    std::unique_lock lock(g_lock);
    LocalizationsProvider::Instance().Reset();  // It is forbidden to load several locales at the same time

    for (auto const& dirEntry : std::filesystem::directory_iterator(cooddyConfigRoot)) {
        auto localePath = dirEntry.path().filename().string();

        if (!StrUtils::EndsWith(localePath, ".loc.json")) {  // .loc.json files has to contain localizations for strings
            continue;
        }

        if (!LoadLocale(dirEntry.path(), locale)) {
            // This failure only possible if something strange happens with file on disk
            // - no permissions to read
            // - file has been deleted between directory traversal and read
            // LCOV_EXCL_START
            ret = false;
            // LCOV_EXCL_STOP
        }
    }
    return ret;
}

}  // namespace StrLocales
}  // namespace HCXX
