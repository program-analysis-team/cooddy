/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <cfg/CfgManager.h>
using namespace HCXX;

namespace HCXX {

void CfgManager::Serialize(IOStream& stream)
{
    if (stream.IsOut()) {
        stream.Add<uint32_t>(myCfgs.size());
        for (auto& it : myCfgs) {
            stream.AddPtr(it.first);
            it.second->Serialize(stream);
        }
    } else {
        auto numCfgs = stream.Get<uint32_t>();
        for (auto i = 0; i < numCfgs; ++i) {
            auto funcDecl = stream.GetPtr<const FunctionDecl*>();
            myCfgs.emplace(funcDecl, std::make_shared<Cfg>(*funcDecl, stream));
        }
    }
}

};  // namespace HCXX
