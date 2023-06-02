/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_DECLRESOLVER_H_
#define COODDY_ANALYZER_INCLUDE_DECLRESOLVER_H_

#include <Parser.h>
#include <TUnitsPackage.h>
#include <ast/Node.h>

namespace HCXX {

class TUnitHandler {
public:
    virtual void TUnitCreated(std::shared_ptr<TranslationUnit> unit) {}
    virtual void TUnitInitDecl(TranslationUnit& unit, const Node* declNode) = 0;
    virtual void TUnitPackage(TUnitsPackage::Ptr package) {}
};

class DeclResolver {
public:
    virtual void AddPackage(TUnitsPackage::Ptr package, TUnitHandler& handler) = 0;

    virtual bool AddDecl(std::string_view declPath, const Node* declNode, bool isInited) = 0;

    virtual DeclResolver* FindResolver(std::string_view subPath, bool waitForInit = true) = 0;

    virtual const Node* FindDecl(std::string_view declPath, bool waitForInit) = 0;

    virtual void InitTUnitData() = 0;

    virtual void SetInited() = 0;

    virtual ~DeclResolver() = default;
};

};  // namespace HCXX

#endif
