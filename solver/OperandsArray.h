/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_OPERANDSARRAY_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_OPERANDSARRAY_H_

class OperandsArray {
public:
    void AddOperands(const std::initializer_list<z3::expr>& operands)
    {
        myOperands.insert(myOperands.end(), operands);
    }

    void ClearOperands()
    {
        myOperands.clear();
    }

    std::optional<z3::expr> GetOperand(uint32_t operandPos)
    {
        if (operandPos >= myOperands.size()) {
            return std::nullopt;  // LCOV_EXCL_LINE
        }
        return myOperands[operandPos];
    }

private:
    std::vector<z3::expr> myOperands;
};

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_OPERANDSARRAY_H_
