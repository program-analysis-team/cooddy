/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_CFG_CFGMANAGER_H_
#define COODDY_ANALYZER_INCLUDE_CFG_CFGMANAGER_H_

#include <ast/FunctionDecl.h>
#include <cfg/Cfg.h>

#include <memory>

namespace HCXX {

class CfgManager {
public:
    using CfgMap = std::unordered_map<const FunctionDecl*, std::shared_ptr<Cfg>>;

    void AddCfg(const FunctionDecl* function, std::shared_ptr<Cfg> cfg)
    {
        myCfgs.emplace(function, std::move(cfg));
    }

    std::shared_ptr<Cfg> GetCfg(const FunctionDecl* function)
    {
        return myCfgs[function];
    }

    const CfgMap& GetCfgs()
    {
        return myCfgs;
    }

    void Init()
    {
        for (auto& item : myCfgs) {
            item.second->Init();
        }
    }

protected:
    ~CfgManager() = default;

    void Clear()
    {
        myCfgs = CfgMap();
    }

    void Serialize(IOStream& stream);

private:
    CfgMap myCfgs;
};

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_CFG_CFGMANAGER_H_
