/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <TranslationUnit.h>
#include <ast/Node.h>

#include <sstream>
#include <string>

namespace HCXX {
void Node::SafePrint(std::string& source, int entryOffset, const std::string& text, SourceRange range, int offset) const
{
    int cur = range.begin + offset - entryOffset;
    if (cur >= 0) {
        for (char c : text) {
            if (cur < source.size() && source[cur] == ' ') {
                source[cur++] = c;
            } else {
                break;
            }
        }
    }
}
void Node::Print(std::string& source, int entryOffset) const
{
    for (Node* node = myChildren; node != nullptr; node = node->myNext) {
        auto range = node->GetRange();
        if (range.begin - entryOffset >= 0 && range.end - entryOffset < source.size()) {
            node->Print(source, entryOffset);
        }
    }
}

// LCOV_EXCL_START
std::string Node::Dump() const
{
    std::stringstream ss;
    ss << Node::GetKindMapping().GetNameByValue(GetKind());
    if (myTU != nullptr && mySourceRange.IsValid()) {
        LocationInfo locInfo = myTU->GetLocationInfo(mySourceRange);
        ss << " (" << locInfo.filename << ":" << locInfo.line << ":" << locInfo.column << " -> " << locInfo.endLine
           << ":" << locInfo.endColumn << ")";
    }
    return ss.str();
}

std::string Node::DumpShort() const
{
    std::stringstream ss;
    ss << "<" << Node::KindToCStr(GetKind());

    std::string_view name = GetName();
    if (!name.empty()) {
        ss << " name:\"" << name << "\"";
    }

    std::string sourceText = GetSourceText();
    if (!sourceText.empty()) {
        ss << " source:\"" << sourceText << "\"";
    }

    ss << ">";
    return ss.str();
}

std::string Node::DumpTree(const Node* root) const
{
    std::stringstream ss;
    if (root == nullptr) {
        root = GetTranslationUnit();
    }
    std::unordered_map<uint32_t, bool> scopes;
    std::function<void(const Node*, uint32_t, bool)> traverse = [&](const Node* node, uint32_t offset,
                                                                    bool lastChildren) {
        if (offset > 0) {
            for (uint32_t i = 1; i < offset; ++i) {
                ss << (scopes[i] ? "| " : "  ");
            }
            ss << (lastChildren ? "`-" : "|-");
        }
        ss << node->Dump() << "\n";
        auto children = node->GetChildren();
        if (!lastChildren) {
            scopes[offset] = true;
        }
        for (uint32_t i = 0; i < children.size(); ++i) {
            traverse(children[i], offset + 1, i == children.size() - 1);
        }
        scopes[offset] = false;
    };
    traverse(root, 0, true);
    return ss.str();
}

void Node::Serialize(IOStream& stream)
{
    uint32_t sizeAndInMacro = 0;
    if (stream.IsOut()) {
        sizeAndInMacro = ((mySourceRange.end - mySourceRange.begin) << 1U) | myInMacro;
    }
    stream << mySourceRange.begin << sizeAndInMacro << myParent << myNext << myChildren;
    if (!stream.IsOut()) {
        myInMacro = sizeAndInMacro & 1U;
        mySourceRange.end = mySourceRange.begin + (sizeAndInMacro >> 1U);
    }
}

void Node::Release() const
{
    if (--myRefCount != 0) {
        return;
    }

    if (myTU == nullptr) {
        Log(LogLevel::ERROR) << "Node is destroying before TU is initialized: " << KindToCStr(GetKind()) << std::endl;
    }

    // COODDY_SUPPRESS
    const_cast<Node*>(this)->ReleaseChildren();
    if (this != myTU) {
        // COODDY_SUPPRESS
        delete this;
    }
}

void Node::ReleaseChildren()
{
    for (Node* node = myChildren; node != nullptr;) {
        Node* toRelease = node;
        node->myParent = nullptr;
        node = node->myNext;
        toRelease->Release();
    }
    myChildren = nullptr;
}

std::string Node::GetSourceText() const
{
    if (myTU != nullptr && mySourceRange.IsValid()) {
        return myTU->GetSourceInRange(mySourceRange);
    }

    return "";
}

// LCOV_EXCL_STOP

}  // namespace HCXX
