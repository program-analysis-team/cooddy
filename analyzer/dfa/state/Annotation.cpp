/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <dfa/Annotation.h>
#include <utils/LocaleUtils.h>

#include <mutex>

namespace HCXX {

std::mutex Annotation::myMutex;
std::vector<uint32_t> Annotation::myTraits;
std::unordered_map<std::string_view, Annotation::Kind> Annotation::myKinds;
std::unordered_set<std::string> Annotation::myKindsKeyStorage;

Annotation::Kind Annotation::RegisterKind(std::string_view kindName, uint32_t traits)
{
    std::unique_lock<std::mutex> lock(myMutex);
    myTraits.reserve(MAX_ANNOTATIONS_COUNT);

    auto it = myKinds.find(kindName);
    if (it == myKinds.end()) {
        // Insert new annotation kind
        kindName = *(myKindsKeyStorage.insert(std::string{kindName}).first);
        it = myKinds.emplace(kindName, 0).first;
    }
    auto& kind = it->second;

    if (kind == 0) {
        kind = myKinds.size();
        myTraits.resize(myKinds.size() + 1);
    }
    myTraits[kind] |= traits;
    return kind;
}

Annotation::Kind Annotation::GetKind(std::string_view kindName)
{
    std::unique_lock<std::mutex> lock(myMutex);
    auto it = myKinds.find(kindName);
    return it == myKinds.end() ? 0 : it->second;
}

std::string_view Annotation::GetKindName(Kind kind)
{
    std::unique_lock<std::mutex> lock(myMutex);
    for (auto& it : myKinds) {
        if (it.second == kind) {
            return it.first;
        }
    }
    // LCOV_EXCL_START
    static std::string empty;
    return empty;
    // LCOV_EXCL_STOP
}

}  // namespace HCXX
