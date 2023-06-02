/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef CONFIGURATION_IMPL_HEADER_FILE
#define CONFIGURATION_IMPL_HEADER_FILE

#include <utils/LocaleUtils.h>
#include <utils/Log.h>
#include <utils/json-cpp.h>

#include <optional>
#include <stdexcept>
#include <string_view>

namespace HCXX {

template <typename TConfiguration>
bool LoadConfiguration(TConfiguration& cfg, const std::unique_ptr<std::istream>& cfgStream, std::string_view configType)
{
    //  LCOV_EXCL_START
    try {
        if (cfgStream == nullptr || !cfgStream->good()) {
            Log(HCXX::LogLevel::ERROR) << "Configuration isn't found for the checker with code " << configType
                                       << std::endl;
            return false;
        }
        jsoncpp::parse(cfg, *cfgStream);
        return true;
    } catch (const std::exception& err) {
        Log(HCXX::LogLevel::ERROR) << "Failed to read configuration for the checker with code " << configType
                                   << ". Error is " << err.what() << "\nStream: '" << cfgStream->rdbuf() << "'"
                                   << std::endl;
    }
    return false;
    // LCOV_EXCL_STOP
}

template <typename TType, typename Converter, typename StreamType>
struct TypeSerializer {
    TypeSerializer(TType& value, Converter converter) : myValue(value), myConverter(converter) {}
    ~TypeSerializer() = default;

    template <class X>
    // TODO HCAT-513
    // COODDY_SUPPRESS CamelCaseChecker
    inline void serialize(jsoncpp::Parser<X>& parser)
    {
        StreamType value;
        jsoncpp::serialize(parser, value);

        std::optional<TType> tmp = myConverter(value);
        if (!tmp.has_value()) {
            //  LCOV_EXCL_START
            Log(HCXX::LogLevel::ERROR) << "Can't convert field value " << parser.getFieldName() << std::endl;
            //  LCOV_EXCL_STOP
        } else {
            myValue = tmp.value();
        }
    }

private:
    TType& myValue;
    Converter myConverter;
};

template <typename TType, typename TypeConverter, typename StreamType = std::string>
TypeSerializer<TType, TypeConverter, StreamType> MakeTypeSerializer(TType& value, TypeConverter converter,
                                                                    StreamType stype = {})
{
    return TypeSerializer<TType, TypeConverter, StreamType>(value, converter);
}

}  // namespace HCXX

#endif  // #ifndef CONFIGURATION_IMPL_HEADER_FILE
