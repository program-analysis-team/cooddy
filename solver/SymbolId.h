/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLID_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLID_H_

#include <ast/VirtualOffset.h>
#include <utils/HashImpl.h>
#include <z3++.h>
using namespace HCXX;

struct StackPos {
    uint16_t pos = 0;
    uint16_t uniqueId = 0;

    bool operator==(const StackPos& p) const
    {
        return uniqueId == p.uniqueId;
    }
    bool operator<(const StackPos& p) const
    {
        return pos < p.pos;
    }
};

struct SymbolId {
    explicit SymbolId(StackPos stackPos, uint32_t varInstr = 0) : myStackPos(stackPos), myVarInstr(varInstr) {}

    explicit SymbolId(const SymbolId& source, VirtualOffset offset) : SymbolId(source)
    {
        myOffsetInVar = offset;
    }

    explicit SymbolId(const std::string& symName)
    {
        char* pend = nullptr;
        myStackPos.pos = std::strtol(symName.c_str(), &pend, 10);
        myStackPos.uniqueId = std::strtol(++pend, &pend, 10);
        myVarInstr = std::strtol(++pend, &pend, 10);
        myOffsetInVar = VirtualOffset(std::strtoll(++pend, &pend, 10));
    }

    ~SymbolId() = default;

    static std::optional<SymbolId> CreateSymbolIdFromExpr(const z3::expr& expr)
    {
        if (expr.is_const() && !expr.is_numeral()) {
            return SymbolId(expr.decl().name().str());
        }
        return std::nullopt;
    }

    static std::optional<z3::expr> CreateSubExpr(const z3::expr& expr, VirtualOffset offset)
    {
        if (offset.IsSourceObject()) {
            return expr;
        }
        auto symbolId = CreateSymbolIdFromExpr(expr);
        if (!symbolId) {
            return std::nullopt;  // LCOV_EXCL_LINE
        }
        return (*symbolId + offset).CreateExpr(expr.ctx());
    }

    z3::expr CreateExpr(z3::context& context) const
    {
        return context.int_const(ToString().c_str());
    }

    bool IsValid() const
    {
        return myVarInstr != 0;
    }
    StackPos GetStackPos() const
    {
        return myStackPos;
    }
    uint32_t GetVarInstr() const
    {
        return myVarInstr;
    }
    VirtualOffset GetOffset() const
    {
        return myOffsetInVar;
    }
    void SetVarInstr(uint32_t varInstr)
    {
        myVarInstr = varInstr;
    }
    void AddSubOffset(VirtualOffset subOffset)
    {
        myOffsetInVar += subOffset;
    }
    bool operator==(const SymbolId& s) const
    {
        return myStackPos == s.myStackPos && myVarInstr == s.myVarInstr && myOffsetInVar == s.myOffsetInVar;
    }
    bool operator!=(const SymbolId& s) const
    {
        return !operator==(s);
    }

    std::string ToString() const
    {
        std::string res = std::to_string(myStackPos.pos) + '.' + std::to_string(myStackPos.uniqueId) + '.';
        res += std::to_string(myVarInstr) + '.' + std::to_string(myOffsetInVar.GetRawValue());
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const SymbolId& id)
    {
        os << id.ToString();
        return os;
    }

    struct Hash {
        std::size_t operator()(const SymbolId& s) const noexcept
        {
            return CalcHash(s.myStackPos.uniqueId, s.myVarInstr, s.myOffsetInVar.GetRawValue());
        }
    };

    bool IsSubSymbolId(const SymbolId& s) const
    {
        return myVarInstr == s.myVarInstr && myStackPos == s.myStackPos && s.myOffsetInVar.IsSubOffsetOf(myOffsetInVar);
    }
    bool IsSubOffset() const
    {
        return !myOffsetInVar.IsSourceObject() && !myOffsetInVar.IsAddrOf();
    }

    friend SymbolId operator+(const SymbolId& symbolId, VirtualOffset offset)
    {
        SymbolId result = symbolId;
        result.AddSubOffset(offset);
        return result;
    }

    bool IsGlobal() const
    {
        return myVarInstr & GLOBAL_VAR_BIT;
    }

    void SetGlobal()
    {
        myVarInstr |= GLOBAL_VAR_BIT;
    }

    bool IsChanged() const
    {
        return myVarInstr & CHANGED_VAR_BIT;
    }

    void SetChanged()
    {
        myVarInstr |= CHANGED_VAR_BIT;
    }

    static void ClearFlags(uint32_t& varInstr)
    {
        varInstr &= ~(GLOBAL_VAR_BIT | CHANGED_VAR_BIT);
    }

    bool IsOutputArg(ArgType argType) const
    {
        return !argType.isConst && argType.isPointer && (argType.isReference || IsSubOffset());
    }

private:
    static const uint32_t GLOBAL_VAR_BIT = (1UL << 23UL);
    static const uint32_t CHANGED_VAR_BIT = (1UL << 24UL);

    StackPos myStackPos;
    uint32_t myVarInstr;
    VirtualOffset myOffsetInVar;
};
#endif  // COODDY_ANALYZER_SOURCE_SOLVER_SYMBOLID_H_
