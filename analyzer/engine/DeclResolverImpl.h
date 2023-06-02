/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_ENGINE_DECLRESOLVERIMPL_H_
#define COODDY_ANALYZER_SOURCE_ENGINE_DECLRESOLVERIMPL_H_

#include <DeclResolver.h>

#include <future>
using namespace HCXX;

class DeclResolverImpl : public DeclResolver {
public:
    void AddPackage(TUnitsPackage::Ptr package, TUnitHandler& handler) override;

    bool AddDecl(std::string_view declPath, const Node* declNode, bool isInited) override;

    DeclResolver* FindResolver(std::string_view subPath, bool waitForInit) override;

    const Node* FindDecl(std::string_view declPath, bool waitForInit) override;

    void InitTUnitData() override
    {
        myPackage->InitTUnit(*myUnit, myEntry);
    }

    void SetInited() override
    {
        myIsInited = true;
    }

private:
    DeclResolverImpl* CreatePackageResolver(const std::string& path);
    DeclResolverImpl* CreateUnitResolver(std::string_view path);
    DeclResolverImpl* GetSubResolver(std::string_view& declPath, bool createIfNotExist = false);
    void WaitForInit();

    TUnitHandler* myHandler = nullptr;
    TUnitsPackage::Ptr myPackage;
    TUnitsPackage::EntryId myEntry = 0;
    NodePtr<Node> myDeclNode;
    std::shared_ptr<TranslationUnit> myUnit;
    std::unordered_map<std::string, std::shared_ptr<DeclResolverImpl>> mySubResolvers;
    std::atomic<uint32_t> myInitCounter = 0;
    bool myIsInited = false;
};

#endif  // COODDY_ANALYZER_SOURCE_ENGINE_DECLRESOLVERIMPL_H_
