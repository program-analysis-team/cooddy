/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_TOOL_CRASHHANDLER_H_
#define COODDY_ANALYZER_TOOL_CRASHHANDLER_H_

void SignalHandler(int sig);
void TerminateHandler();

void SetCrashHandler();

#endif  // COODDY_ANALYZER_TOOL_CRASHHANDLER_H_
