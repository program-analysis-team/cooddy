/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Declaration of base class for AST-nodes.
//

#ifndef HCXXANALYZER_NODE_H
#define HCXXANALYZER_NODE_H

#include <ast/Type.h>
#include <utils/EnumMapping.h>
#include <utils/Memory.h>

#include <algorithm>
#include <atomic>
#include <climits>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace HCXX {

#define CALL_CALLBACK(node, callback) \
    if (node != nullptr) {            \
        callback(*node);              \
    }

class TranslationUnit;
struct LocationInfo;

using Location = uint32_t;
using TraverseCallback = const std::function<void(const Node&)>&;
using TraverseDeclCallback = const std::function<void(const Node&, uint32_t)>&;

/**
 * Source range describe node start inclusively and node end exclusively,
 * ie: for a 1 char node, end - begin = 1;
 */
struct SourceRange {
    Location begin = 0;
    Location end = 0;

    void Serialize(IOStream& stream)
    {
        stream << begin << end;
    }
    uint32_t Size() const
    {
        return end - begin;
    }
    bool operator==(const SourceRange& rhs) const
    {
        return begin == rhs.begin && end == rhs.end;
    };
    bool operator!=(const SourceRange& rhs) const
    {
        return !(*this == rhs);
    };
    bool operator<(const SourceRange& rhs) const
    {
        return begin < rhs.begin || (begin == rhs.begin && end > rhs.end);
    }
    bool IsValid() const
    {
        return begin != 0 && end != 0;
    }
    bool Contains(const SourceRange& other) const
    {
        return begin <= other.begin && end >= other.end;
    }

    friend std::ostream& operator<<(std::ostream& os, const SourceRange& obj)
    {
        os << obj.begin << ":" << obj.end;

        return os;
    }
};

class Node {
public:
    DECLARE_ENUM(Kind, NODE, UNDEFINED, TRANSLATION_UNIT, BINARY_EXPRESSION, CAST_EXPRESSION, NAMESPACE,
                 UNARY_EXPRESSION, LITERAL_EXPRESSION, IF_STATEMENT, ELSE_KEYWORD, CALL_EXPRESSION,
                 CONDITIONAL_EXPRESSION, REF_EXPRESSION, FUNCTION_DECL, STATIC_ASSERT_DECL, RETURN_STATEMENT,
                 CXX_RECORD_DECL, VAR_DECL, PARAM_VAR_DECL, TRY_EXPRESSION, THROW_EXPRESSION, CATCH_EXPRESSION,
                 DELETE_EXPRESSION, THIS_EXPRESSION, PAREN_EXPRESSION, COMPOUND_STATEMENT, DECL_STATEMENT,
                 LOOP_STATEMENT, SWITCH_STATEMENT, SWITCH_CASE_STATEMENT, CASE_STATEMENT, DEFAULT_STATEMENT,
                 CXX_CTOR_DECL, FIELD_DECL, MEMBER_EXPRESSION, CXX_MEMBER_CALL_EXPRESSION, CXX_OPERATOR_CALL_EXPRESSION,
                 CXX_METHOD_DECL, CXX_DEFAULT_ARG_EXPRESSION, LABEL_STATEMENT, GOTO_STATEMENT, BREAK_STATEMENT,
                 TYPEDEF_DECL, TYPE_ALIAS_DECL, ENUM_DECL, ENUM_CONSTANT_DECL, INIT_LIST_EXPRESSION,
                 IMPLICIT_VALUE_INIT_LIST_EXPRESSION, TYPED_NODE, TEMPORARY_EXPRESSION, ARRAY_SUBSCRIPT_EXPRESSION,
                 LAMBDA_EXPRESSION, CXX_CONVERSION_DECL, ACCESS_SPEC_DECL, ASM_STATEMENT, DECL_NAME, DECL_QUALIFIER,
                 CXX_OPERATOR_TOKEN, INT_LITERAL_EXPRESSION, CXX_NEW_EXPRESSION, EMPTY_STMT, STRING_LITERAL_EXPRESSION,
                 CXX_CONSTRUCT_EXPRESSION, BOOL_LITERAL_EXPRESSION, CXX_CTOR_INITIALIZER, RECORD_DECL, CXX_DTOR_DECL,
                 CXX_DTOR_EXPRESSION, CONTINUE_STATEMENT, CHAR_LITERAL_EXPRESSION, FLOAT_LITERAL_EXPRESSION);

    /// Dump detailed node info taking up an entire string
    virtual std::string Dump() const;

    /// Dump short node info for including in the middle of a line.
    std::string DumpShort() const;

    /// Dump detailed AST tree
    std::string DumpTree(const Node* root = nullptr) const;

    const TranslationUnit* GetTranslationUnit() const
    {
        return myTU;
    }

    const Node* GetParent() const
    {
        return myParent;
    }

    template <class TNode>
    const TNode* GetParentOfType() const
    {
        for (const Node* parent = GetParent(); parent != nullptr; parent = parent->GetParent()) {
            if (parent->IsKindOf(TNode::KIND)) {
                return static_cast<const TNode*>(parent);
            }
        }
        return nullptr;
    }

    template <class TNode = Node>
    std::vector<const TNode*> GetChildren(bool recursively = false) const
    {
        std::vector<const TNode*> result;
        GetChildren<TNode>(recursively, result);
        std::reverse(result.begin(), result.end());
        return result;
    }

    const Node* GetChild() const
    {
        return myChildren;
    }

    // LCOV_EXCL_START
    virtual std::string GetFQN() const
    {
        return GetName();
    }

    virtual const std::string& GetName() const
    {
        static const std::string empty = "";
        return empty;
    }
    // LCOV_EXCL_STOP

    const SourceRange& GetRange() const
    {
        return mySourceRange;
    }

    void SetRange(const SourceRange& range)
    {
        mySourceRange = range;
    }

    std::string GetSourceText() const;

    bool InRange(const SourceRange& sourceRange) const
    {
        return sourceRange.Contains(mySourceRange);
    }

    bool IsSystem() const
    {
        return myIndexPos == 0;
    }

    /**
     * @brief Determine if this node intersects a macro expansion in some way.
     *
     * Returns true if node beginning and/or end is inside a macro expansion.
     *
     * Examples of return values for nodes:
     *   "<macro>" -> true
     *   "<code> op <macro>" -> true
     *   "<macro> op <code> op <macro>" -> true
     *   "<code> op <macro> op <code>" -> false
     */
    bool InMacro() const
    {
        return myInMacro;
    }

    void SetInMacro(bool b)
    {
        myInMacro = b;
    }

    void AddRef() const
    {
        ++myRefCount;
    }

    void Release() const;

    void ReleaseChildren();

    uint32_t GetSizeOfType() const;

    virtual void Init() {}

    virtual void Clear()
    {
        for (Node* node = myChildren; node != nullptr; node = node->myNext) {
            node->Clear();
        }
    }

    virtual Type GetType() const
    {
        return Type();
    }

    virtual void Serialize(IOStream& stream);

    virtual std::optional<std::string> GetAttribute(std::string_view attrName) const
    {
        return std::nullopt;
    }

    virtual bool IsDeclaration() const
    {
        return false;
    }

    template <class TNode = Node>
    const TNode* CompareAndCast(const Node* comparedNode, Kind excludedKind = Kind::UNDEFINED) const
    {
        if (!comparedNode->IsKindOf(TNode::KIND)) {
            return nullptr;
        }
        const auto& comparedChildren = comparedNode->GetChildren();
        const auto& children = GetChildren();
        if (children.size() != comparedChildren.size()) {
            return nullptr;  // LCOV_EXCL_LINE: HCAT-2779
        }
        for (int i = 0; i < children.size(); ++i) {
            if (!comparedChildren[i]->IsKindOf(excludedKind) && !children[i]->IsEqualNode(comparedChildren[i])) {
                return nullptr;
            }
        }
        return Node::Cast<TNode>(comparedNode);
    }

    virtual bool IsEqualNode(const Node* comparedNode) const
    {
        return comparedNode != nullptr && Type() == comparedNode->GetType() &&
               (comparedNode == this || CompareAndCast(comparedNode) != nullptr);
    }

    virtual void Traverse(TraverseCallback callback) const
    {
        TraverseCallback cb = [&](const Node& node) { node.Traverse(callback); };
        TraverseChildren(cb);
        callback(*this);
    }

    virtual void TraverseChildren(TraverseCallback callback) const
    {
        std::vector<Node*> result;
        for (Node* node = myChildren; node != nullptr; node = node->myNext) {
            result.emplace_back(node);
        }

        for (auto it = result.crbegin(); it != result.crend(); ++it) {
            callback(**it);
        }
    }

    enum UsageFlags { READ = 0, WRITE = 1, INCREMENT = 2, DECREMENT = 4 };

    virtual void TraverseDeclUsage(TraverseDeclCallback callback, uint32_t usageFlags = 0) const
    {
        TraverseCallback cb = [&](const Node& node) { node.TraverseDeclUsage(callback); };
        TraverseChildren(cb);
    }

    virtual const Node* GetInnerNode() const
    {
        return this;
    }

    template <class T>
    static const T* Cast(const Node* node)
    {
        return node != nullptr && node->IsKindOf(T::KIND) ? static_cast<const T*>(node) : nullptr;
    }

    template <class T>
    static T* Cast(Node* node)
    {
        return node != nullptr && node->IsKindOf(T::KIND) ? static_cast<T*>(node) : nullptr;
    }

    virtual bool IsKindOf(Kind kind) const
    {
        return kind == Kind::NODE;
    }

    static const Kind KIND = Kind::NODE;

    // To implement this method use DECLARE_KIND macro
    virtual Kind GetKind() const = 0;

    virtual ~Node() = default;

    // LCOV_EXCL_START
    /// Output operator to simplify debugging.
    friend std::ostream& operator<<(std::ostream& os, const Node& node)
    {
        os << node.DumpShort();
        return os;
    }
    // LCOV_EXCL_STOP

    virtual void Print(std::string& source, int entryOffset) const;
    void SafePrint(std::string& source, int entryOffset, const std::string& text, SourceRange range,
                   int offset = 0) const;

    virtual void GetMemorySize(MemCounter& counter) const
    {
        counter << *this;
    }

protected:
    explicit Node() : myIndexPos(0), myInMacro(0) {}
    Node(const Node& other) : myIndexPos(other.myIndexPos), myInMacro(other.myInMacro) {}

private:
    template <class TNode>
    void GetChildren(bool recursively, std::vector<const TNode*>& result) const
    {
        for (Node* node = myChildren; node != nullptr; node = node->myNext) {
            if (node->IsKindOf(TNode::KIND)) {
                result.emplace_back(static_cast<const TNode*>(node));
            }

            // LCOV_EXCL_START
            if (recursively) {
                node->GetChildren<TNode>(recursively, result);
            }
            // LCOV_EXCL_STOP
        }
    }

    TranslationUnit* myTU{};
    SourceRange mySourceRange{};
    Node* myParent{};
    Node* myNext{};
    Node* myChildren{};
    uint32_t myIndexPos : 31;
    uint32_t myInMacro : 1;
    mutable std::atomic<uint32_t> myRefCount;

    friend class AstManager;
    friend class TranslationUnit;
};

#define DECLARE_KIND(base, kind)                             \
    using Base = base;                                       \
    static const Node::Kind KIND = kind;                     \
    bool IsKindOf(Node::Kind _kind) const override           \
    {                                                        \
        return _kind != KIND ? base::IsKindOf(_kind) : true; \
    }                                                        \
    Node::Kind GetKind() const override                      \
    {                                                        \
        return KIND;                                         \
    }

extern void RegisterNodeCreator(Node::Kind kind, std::function<Node*()> creator);

#define DECLARE_SERIALIZE(type, body)                                       \
    type() {}                                                               \
    static void RegisterCreator()                                           \
    {                                                                       \
        HCXX::RegisterNodeCreator(type::KIND, []() { return new type(); }); \
    }                                                                       \
    void Serialize(IOStream& stream) override                               \
    {                                                                       \
        stream << body;                                                     \
        return Base::Serialize(stream);                                     \
    }                                                                       \
    virtual void GetMemorySize(MemCounter& counter) const                   \
    {                                                                       \
        counter << body;                                                    \
        Base::GetMemorySize(counter);                                       \
    }

template <class TNode>
class NodePtr {
public:
    //  We suppress the rule CSR_7.1.2 "Declare single-parameter constructors as explicit to prevent implicit
    //      conversion"
    //  The ctor is made implicit by design, it requires implicit cast from const Node pointer
    //  (c)Pavel Mezhuev 00565231
    //  COODDY_SUPPRESS HastBasedChecker
    NodePtr(const TNode* node = nullptr) : myNode(node)
    {
        AddRef();
    }
    NodePtr(const NodePtr& node) : myNode(node.myNode)
    {
        AddRef();
    }
    NodePtr(NodePtr&& node)
    {
        myNode = node.myNode;
        node.myNode = nullptr;
    }
    ~NodePtr()
    {
        Release();
    }
    void operator=(NodePtr&& node)
    {
        Release();
        myNode = node.myNode;
        node.myNode = nullptr;
    }
    void operator=(const NodePtr& node)
    {
        Release();
        myNode = node.myNode;
        AddRef();
    }
    operator const TNode*() const
    {
        return myNode;
    }
    const TNode* operator->() const
    {
        return myNode;
    }

private:
    void Release()
    {
        if (myNode != nullptr) {
            myNode->Release();
        }
    }
    void AddRef()
    {
        if (myNode != nullptr) {
            myNode->AddRef();
        }
    }

private:
    const TNode* myNode;
};

template <class T>
void Serialize(IOStream& stream, NodePtr<T>& node)
{
    if (stream.IsOut()) {
        stream.AddPtr(node);
    } else {
        node = stream.GetPtr<const T*>();
    }
}

};  // namespace HCXX

#endif  // HCXXANALYZER_NODE_H
