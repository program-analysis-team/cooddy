/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include "DeclResolverImpl.h"

#include <utils/EnvironmentUtils.h>

#include <thread>

void DeclResolverImpl::AddPackage(TUnitsPackage::Ptr package, TUnitHandler& handler)
{
    std::unordered_set<std::string> entryNames;
    // LCOV_EXCL_START: GCOV bug
    std::function<void(DeclResolverImpl&, TUnitsPackage::Ptr&)> traversePackage = [&](DeclResolverImpl& resolver,
                                                                                      TUnitsPackage::Ptr& package) {
        // LCOV_EXCL_STOP
        myHandler = &handler;
        myHandler->TUnitPackage(package);
        for (uint32_t i = 0, n = package->GetEntriesCount(); i < n; i++) {
            auto entryName = EnvironmentUtils::NormalizePath(package->GetEntryName(i));
            if (auto subPackage = package->GetPackage(i); subPackage) {
                traversePackage(*resolver.CreatePackageResolver(entryName), subPackage);
                continue;
            }
            if (!entryNames.emplace(entryName).second) {
                continue;
            }
            auto unit = package->GetTUnit(i);
            if (unit == nullptr) {
                continue;  // LCOV_EXCL_LINE
            }
            auto subResolver = resolver.CreateUnitResolver(entryName);
            if (subResolver == nullptr) {
                continue;  // LCOV_EXCL_LINE
            }
            subResolver->myUnit = std::move(unit);
            subResolver->myPackage = package;
            subResolver->myEntry = i;
            subResolver->myUnit->SetResolver(subResolver);
            myHandler->TUnitCreated(subResolver->myUnit);
        }
    };
    traversePackage(*this, package);
}

DeclResolverImpl* DeclResolverImpl::CreatePackageResolver(const std::string& packagePath)
{
    if (std::filesystem::is_directory(packagePath)) {
        return this;
    }
    DeclResolverImpl* result = this;
    for (auto& entry : StrUtils::Split(packagePath, '/')) {
        result = result->GetSubResolver(entry, true);
    }
    return result;
}

DeclResolverImpl* DeclResolverImpl::CreateUnitResolver(std::string_view unitPath)
{
    DeclResolverImpl* result = this;
    for (auto entry : StrUtils::Split(unitPath, '/')) {
        auto p = entry.find('.');
        if (p != std::string::npos) {
            entry = std::string_view(entry.data(), p);
        }
        result = result->GetSubResolver(entry, true);
    }
    return result;
}

DeclResolverImpl* DeclResolverImpl::GetSubResolver(std::string_view& declPath, bool createIfNotExist)
{
    auto p = declPath.find('.');
    std::string key = std::string(declPath.substr(0, p));
    declPath = p == std::string::npos ? "" : declPath.substr(p + 1);
    auto entry = mySubResolvers.find(key);
    if (entry == mySubResolvers.end()) {
        if (!createIfNotExist) {
            return nullptr;
        }
        entry = mySubResolvers.emplace(key, std::make_unique<DeclResolverImpl>()).first;
    }
    if (createIfNotExist) {
        entry->second->myHandler = myHandler;
        entry->second->myUnit = myUnit;
    }
    return entry->second.get();
}

bool DeclResolverImpl::AddDecl(std::string_view declPath, const Node* declNode, bool isInited)
{
    if (declPath.empty()) {
        myIsInited = isInited;
        myDeclNode = declNode;
        return true;
    }
    auto resolver = GetSubResolver(declPath, myUnit != nullptr || myDeclNode != nullptr);
    return resolver != nullptr && resolver->AddDecl(declPath, declNode, isInited);
}

const Node* DeclResolverImpl::FindDecl(std::string_view declPath, bool waitForInit)
{
    auto resolver = static_cast<DeclResolverImpl*>(FindResolver(declPath, waitForInit));
    return resolver != nullptr ? resolver->myDeclNode : nullptr;
}

DeclResolver* DeclResolverImpl::FindResolver(std::string_view subPath, bool waitForInit)
{
    if (subPath.empty() || subPath == "*") {
        if (waitForInit) {
            WaitForInit();
        }
        return this;
    }
    WaitForInit();
    auto resolver = GetSubResolver(subPath, false);
    return resolver != nullptr ? resolver->FindResolver(subPath, waitForInit) : nullptr;
}

void DeclResolverImpl::WaitForInit()
{
    if (myUnit == nullptr) {
        return;
    }
    if (!myIsInited && ++myInitCounter == 1) {
        myHandler->TUnitInitDecl(*myUnit, myDeclNode);
    }
    while (!myIsInited) {
        std::this_thread::yield();  // LCOV_EXCL_LINE
    }
}
