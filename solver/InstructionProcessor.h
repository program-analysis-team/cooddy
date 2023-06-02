/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_INSTRUCTIONPROCESSOR_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_INSTRUCTIONPROCESSOR_H_

#include <cfg/CfgBlock.h>
#include <solver/FunctionBehavior.h>

#include <limits>

#include "SymbolId.h"

using namespace HCXX;
class FunctionBehaviorImpl;
class ExecutionContext;

struct InstructionHeader {
    uint32_t kind : 7;
    uint32_t isCached : 1;
    uint32_t rangeOffset : 16;
    uint32_t rangeSize : 8;
};

class InstructionProcessor {
public:
    struct CompileContext {
        FunctionBehaviorImpl& owner;
        DataStream& instrSet;
        uint32_t curDepth = 0;

        void Compile(const Node* node);

        template <class T>
        void Add(T val)
        {
            instrSet.Add<T>(val);
        }
        template <class T>
        void AddEnum(T val)
        {
            static_assert(static_cast<int64_t>(std::numeric_limits<T>::min()) >= std::numeric_limits<uint8_t>::min());
            static_assert(static_cast<int64_t>(std::numeric_limits<T>::max()) <= std::numeric_limits<uint8_t>::max());
            instrSet.Add<uint8_t>(static_cast<uint8_t>(val));
        }
        void AddString(const char* str)
        {
            instrSet.AddString(str);
        }
    };

    virtual void Compile(const Node& node, CompileContext& context) {}

    struct ExecutionContext {
        DataStream& instrSet;
        uint32_t curInstr;
        ::ExecutionContext& context;
        ExecutionCallback callback;
        Instruction instruction = 0;
        std::vector<std::pair<SymbolId, int32_t>> postIncs;

        z3::expr Execute(SymbolId* symbolId = nullptr);

        template <class T>
        T Get()
        {
            return instrSet.Get<T>(&curInstr);
        }
        template <class T>
        T GetEnum()
        {
            return static_cast<T>(instrSet.Get<uint8_t>(&curInstr));
        }
        const char* GetString()
        {
            return instrSet.GetString(&curInstr);
        }
        ::ExecutionContext* operator->() const
        {
            return &context;
        }
        void ExecutePostOperations();
    };

    virtual z3::expr Execute(ExecutionContext& context, SymbolId& symbolId);

    virtual void PostExecute(ExecutionContext& context, z3::expr& result) {}

    static InstructionProcessor& GetProcessor(Node::Kind kind);
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_INSTRUCTIONPROCESSOR_H_
