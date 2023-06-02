/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <dfa/Annotation.h>
#include <dfa/DfaFunctionContext.h>
#include <dfa/FormatStringKind.h>
#include <solver/SolverContext.h>
#include <utils/StrUtils.h>
#include <utils/json-cpp.h>
#include <workspace/Workspace.h>

#include <cctype>
#include <filesystem>
#include <fstream>

#include "CompilerOptionsList.h"

namespace HCXX {

template <class X>
// TODO HCAT-513
// COODDY_SUPPRESS CamelCaseChecker
void serialize(jsoncpp::Parser<X>& p, Annotation& annotation)
{
    if (p.getType() != jsoncpp::Type::String) {
        return;
    }
    std::string value;
    jsoncpp::serialize(p, value);
    auto annotationInfo = StrUtils::Split(value, ':', false);

    VirtualOffset offset;
    uint32_t argPos = Annotation::ArgInfo::MAX_ARG_POS;
    auto kind = Annotation::RegisterKind(annotationInfo[0], 0);
    if (annotationInfo.size() <= 1) {
        annotation = {kind, Annotation::ArgInfo(argPos), 0, offset};
        return;
    }

    if (annotationInfo[0] == "Alias") {
        // Get node argument position from the end of the annotation value
        argPos = StrUtils::ToInteger<uint32_t>(annotationInfo[2]);
    } else {
        if (annotationInfo.size() > 2) {
            if (annotationInfo[0] == "FormatString") {
                annotation.GetUserData<FormatStringKind>() = FormatStringKindFromStr(annotationInfo[2]);
            } else if (annotationInfo[0] == "Result" || annotationInfo[0] == "Condition") {
                annotation.GetUserData() = StrUtils::GetStringId(annotationInfo[2]);
            } else if (std::isdigit(annotationInfo[2][0]) || annotationInfo[2][0] == '-') {
                annotation.GetUserData<>() = StrUtils::ToInteger<int64_t>(annotationInfo[2]);
            } else {
                annotation.GetUserData<Condition>().FromString(annotationInfo[2]);
            }
        }
    }
    offset = VirtualOffset(annotationInfo[1]);
    annotation = {kind, Annotation::ArgInfo(argPos), 0, offset, annotation.GetUserData<>()};
}
// LCOV_EXCL_START
template <class X>
// TODO HCAT-513
// COODDY_SUPPRESS CamelCaseChecker
void serialize(jsoncpp::Generator<X>& p, Annotation& annotation)
{
    std::string value{Annotation::GetKindName(annotation.GetKind())};
    if (value == "Alias") {
        value += ":" + annotation.GetMemoryOffset().ToString() + ":" + std::to_string(annotation.GetArgPos());
    } else if (!annotation.GetMemoryOffset().IsSourceObject() || annotation.GetUserData<>() != 0) {
        value += ":" + annotation.GetMemoryOffset().ToString();
        int64_t userData = annotation.GetUserData<int64_t>();
        if (userData != 0) {
            if (userData < INT32_MAX) {
                value += ":" + std::to_string(userData);
            } else {
                value += ":" + annotation.GetUserData<Condition>().ToString();
            }
        }
    }
    serialize(p, value);
}
// LCOV_EXCL_STOP
void Workspace::LoadAnnotations(const std::string& annotationsPath, UnitLanguage lang)
{
    if (annotationsPath.empty()) {
        return;
    }
    myConfigurationFileNames.push_back(annotationsPath);
    try {
        std::unordered_map<std::string, FunctionAnnotation> annotations;
        std::ifstream annotationsStream(annotationsPath);
        jsoncpp::parse(annotations, annotationsStream);
        for (auto& it : annotations) {
            size_t p = it.first.rfind('(');
            if (p == std::string::npos) {
                myAnnotations[lang].insert_or_assign(it.first, FunctionInfo{it.first, std::move(it.second)});
            } else {
                myAnnotations[lang].insert_or_assign(it.first.substr(p + 1, it.first.size() - p - 2),
                                                     FunctionInfo{it.first.substr(0, p), std::move(it.second)});
            }
        }
        // LCOV_EXCL_START
    } catch (std::exception& ex) {
        Log(LogLevel::ERROR) << "Can't load annotations: '" << annotationsPath << "' " << ex.what() << std::endl;
    }
    // LCOV_EXCL_STOP
    Log(LogLevel::DEBUG) << "Annotations are loaded from: '" << annotationsPath << "'" << std::endl;
}

void Workspace::SaveAnnotations(const std::unordered_map<string, FunctionInfo>& annoToExport,
                                const std::filesystem::path& path)
{
    std::map<std::string, FunctionAnnotation> annotations;
    for (auto& it : annoToExport) {
        annotations.emplace(it.second.displayName + '(' + it.first + ')', it.second.annotations);
    }

    auto filepath = GetFilePath(path, false);
    std::ofstream outStream(filepath);
    outStream << jsoncpp::to_string(annotations);
    Log(LogLevel::INFO) << "File with generated annotations: " << filepath << std::endl;
}

bool Workspace::IsAnnotationShouldBeGenerated(const DfaFunctionContext* funcContext)
{
    auto flag = myOptions.genAnnoSettings.genAnnoFlags;
    bool shouldBeGeneratedAnno =
        ((flag & GenAnnoSettings::GenAnnoFlags::SYSTEM) && funcContext->IsSystem()) ||
        ((flag & GenAnnoSettings::GenAnnoFlags::UNDEFINED) && !funcContext->IsSystem() && funcContext->IsUndefined()) ||
        ((flag & GenAnnoSettings::GenAnnoFlags::ANNOTATED) && funcContext->IsTaintProblemFound());

    FunctionAnnotation annotation;
    bool hasNoPreloadAnno = !GetAnnotation(funcContext->GetSignature(), annotation, funcContext->GetLanguage());

    return shouldBeGeneratedAnno && hasNoPreloadAnno;
}

void Workspace::GenerateFunctionAnnotations(std::vector<const DfaFunctionContext*>& globalFuncs)
{
    if (myOptions.genAnnoSettings.IsDisabled()) {
        return;
    }

    std::unordered_map<std::string, FunctionInfo> funcsToExport;
    for (auto& func : globalFuncs) {
        if (IsAnnotationShouldBeGenerated(func)) {
            auto& funcInfo = funcsToExport[func->GetSignature()];
            funcInfo.displayName = func->GetFQN();
            if (funcInfo.annotations.empty()) {
                for (size_t i = 0; i <= func->GetSizeOfAnnotations(); ++i) {
                    funcInfo.annotations.emplace_back(ParamAnnotation());
                }
            }
        }
    }
    SaveAnnotations(funcsToExport, myOptions.genAnnoPath);
}

bool Workspace::GetAnnotation(const std::string& function, FunctionAnnotation& annotation, UnitLanguage lang)
{
    if (!IsDefault() && myDefaultWorkspace && myDefaultWorkspace->GetAnnotation(function, annotation, lang)) {
        return true;
    }
    if (lang == UnitLanguage::CPP) {
        lang = UnitLanguage::C;
    }
    auto it = myAnnotations[lang].find(function);
    if (it != myAnnotations[lang].end()) {
        annotation = it->second.annotations;
        return true;
    }
    return false;
}

void Workspace::SetEnabledCheckers(const std::unordered_set<string>& checkerNames)
{
    myProfile->SetEnabledCheckers(checkerNames);
}
};  // namespace HCXX
