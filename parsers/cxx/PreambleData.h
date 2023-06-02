/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// class PreambleData allows using of clang::PrecompiledPreamble to increase AST parsing speed
//
#ifndef COODDY_ANALYZER_SOURCE_CLANG_FACADE_PREAMBLE_H_
#define COODDY_ANALYZER_SOURCE_CLANG_FACADE_PREAMBLE_H_

#include <Parser.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/PrecompiledPreamble.h>

class FileSystemProxy;

class PreambleData {
public:
    explicit PreambleData(FileSystemProxy* fileSystem) : myFileSystem(fileSystem){};
    ~PreambleData() = default;

    bool Apply(clang::CompilerInvocation& ci, clang::IntrusiveRefCntPtr<llvm::vfs::FileSystem>& vfs,
               llvm::MemoryBuffer& memBuffer, HCXX::Parser::Consumer& consumer);

    bool Build(clang::CompilerInvocation& ci, llvm::MemoryBuffer& memBuffer, HCXX::Parser::Consumer& consumer);

private:
    FileSystemProxy* myFileSystem;
    std::unique_ptr<clang::PrecompiledPreamble> myPreamble;
};

#endif  // COODDY_ANALYZER_SOURCE_CLANG_FACADE_PREAMBLE_H_
