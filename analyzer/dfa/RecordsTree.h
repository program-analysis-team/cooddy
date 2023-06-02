/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_DFA_RECORDSMAP_H_
#define COODDY_ANALYZER_SOURCE_DFA_RECORDSMAP_H_

#include <ast/CxxMethodDecl.h>
#include <ast/CxxRecordDecl.h>

using namespace HCXX;

class FunctionContext;

class RecordsTree {
public:
    struct Node {
        uint32_t orderPos = 0;
        RecordUniqueId recordId = 0;
        std::vector<Node*> baseRecords;
        std::unordered_map<std::string, FunctionContext*> virtualMethods;
    };

    std::optional<Node*> AddRecord(RecordUniqueId recordId, const std::vector<RecordUniqueId>& baseRecords)
    {
        std::unique_lock<std::mutex> lock(myMutex);
        auto& node = myRecords[recordId];
        if (node.recordId != 0) {
            return std::nullopt;  // already inited
        }
        node.recordId = recordId;
        node.orderPos = myCurOrderPos++;
        for (auto& base : baseRecords) {
            auto& b = node.baseRecords.emplace_back(&myRecords[base]);
            node.baseRecords.insert(node.baseRecords.end(), b->baseRecords.begin(), b->baseRecords.end());
        }
        return &node;
    }

    Node& AddRecordRaw(RecordUniqueId recordId, const std::vector<RecordUniqueId>& baseRecords)
    {
        auto& node = myRecords[recordId];
        node.recordId = recordId;
        node.orderPos = myCurOrderPos++;
        for (auto& base : baseRecords) {
            node.baseRecords.emplace_back(&myRecords[base]);
        }
        return node;
    }

    const Node* FindRecord(RecordUniqueId recordId) const
    {
        std::unique_lock<std::mutex> lock(myMutex);
        auto it = myRecords.find(recordId);
        return it != myRecords.end() ? &it->second : nullptr;
    }

    auto& GetRecords() const
    {
        return myRecords;
    }

private:
    std::unordered_map<RecordUniqueId, Node> myRecords;
    mutable std::mutex myMutex;
    uint32_t myCurOrderPos = 0;
};

#endif  // COODDY_ANALYZER_SOURCE_DFA_RECORDSMAP_H_
