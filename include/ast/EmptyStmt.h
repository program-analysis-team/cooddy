/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef AST_EMPTY_STMT_HEADER_FILE
#define AST_EMPTY_STMT_HEADER_FILE

#include "Node.h"

namespace HCXX {

//  The empty statement. It may appear
//  - as explicit node(Developer used extra semicolon)
//  - as implicit node(Templates instantiation)
//  Example 1:
//  void Foo()
//  {
//      Goo();  //  Semicolon is NOT an empty statement, it is a part of another statement
//  }
//  Example 2:
//  void Foo()
//  {
//      Goo();;  //  Additional semicolon is an empty statement
//  }
//  Example 3:
//  template <bool UseAdd>
//  int FooImpl(int x, int y)
//  {
//      if constexpr (UseAdd) {
//          //  If FooImpl<false> is used than '{return x + y}' is replaced with empty statement
//          return x + y;
//      } else {
//          //  If FooImpl<true> is used than '{return x - y}' is NOT replaced with empty statement
//          //  Clang treats if statement as 'if without else statement'
//          return x - y;
//      }
//  }
class EmptyStmt : public HCXX::Node {
public:
    // LCOV_EXCL_START
    DECLARE_KIND(Node, Node::Kind::EMPTY_STMT);
    DECLARE_SERIALIZE(EmptyStmt, nullptr);
    // LCOV_EXCL_STOP

    //  The method returns true if the empty statement appeared as replacement of some statement(s)
    //  with empty statement as result of template  instantiation
    bool CheckIfSyntheticStmt() const;
};

}  // namespace HCXX

#endif  // #ifndef AST_EMPTY_STMT_HEADER_FILE
