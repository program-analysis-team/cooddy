/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// Class, Union, Struct.
//

#ifndef COODDY_CXXRECORDDECL_H
#define COODDY_CXXRECORDDECL_H

#include <TranslationUnit.h>
#include <ast/CxxCtorDecl.h>
#include <ast/CxxDtorDecl.h>
#include <ast/FieldDecl.h>
#include <ast/NamedNode.h>
#include <ast/Node.h>

#include <vector>

namespace HCXX {

class FunctionDecl;

class RecordDecl : public NamedNode<Node> {
public:
    using Fields = std::vector<NodePtr<FieldDecl>>;

    DECLARE_ENUM(RecordType, CLASS, STRUCT, UNION, UNKNOWN);

    DECLARE_KIND(NamedNode<Node>, Node::Kind::RECORD_DECL);

    DECLARE_SERIALIZE(RecordDecl, myFields << myRecordType << myUniqueId << myIsFirstDeclaration << myIsImplicit
                                           << mySizeInBits << myCommentBlocks << myIsSensitive);

    RecordDecl(Node* qualifier, std::string&& name, Fields&& fields, RecordType recordType, bool isFirstDeclaration,
               bool isImplicit, uint64_t sizeInBits, std::vector<SourceRange>&& commentBlocks, bool isSensitive = false)
        : Base(qualifier, std::move(name)),
          myFields(std::move(fields)),
          myRecordType(recordType),
          myIsFirstDeclaration(isFirstDeclaration),
          myIsImplicit(isImplicit),
          mySizeInBits(sizeInBits),
          myCommentBlocks(std::move(commentBlocks)),
          myIsSensitive(isSensitive)
    {}

    const Fields& GetFields() const
    {
        return myFields;
    }

    void SetFields(Fields&& fields);

    bool IsSensitive() const
    {
        return myIsSensitive;
    }
    bool IsUnion() const
    {
        return myRecordType == RecordType::UNION;
    }

    // LCOV_EXCL_START
    bool IsClass() const
    {
        return myRecordType == RecordType::CLASS;
    }

    bool IsStruct() const
    {
        return myRecordType == RecordType::STRUCT;
    }
    // LCOV_EXCL_STOP
    bool IsAnonymous() const
    {
        return GetName().empty();
    }

    bool IsFirstDeclaration() const
    {
        return myIsFirstDeclaration;
    }

    bool IsImplicit() const
    {
        return myIsImplicit;
    }

    // Returns cross unit unique identifier for this declaration
    RecordUniqueId GetUniqueId() const
    {
        return myUniqueId;
    }
    // LCOV_EXCL_START
    bool IsDeclaration() const override
    {
        return true;
    }

    std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "IsUnion") {
            return std::to_string(IsUnion());
        }
        if (attrName == "IsClass") {
            return std::to_string(IsClass());
        }
        if (attrName == "IsStruct") {
            return std::to_string(IsStruct());
        }
        if (attrName == "IsAnonymous") {
            return std::to_string(IsAnonymous());
        }
        if (attrName == "IsFirstDeclaration") {
            return std::to_string(IsFirstDeclaration());
        }
        if (attrName == "IsImplicit") {
            return std::to_string(IsImplicit());
        }
        return Base::GetAttribute(attrName);
    }

    void TraverseChildren(TraverseCallback callback) const override
    {
        for (auto& node : myFields) {
            CALL_CALLBACK(node, callback);
        }
    }

    // LCOV_EXCL_STOP

    void Init() override;

    void Clear() override
    {
        Base::Clear();
        ReleaseChildren();
    }

    uint64_t GetSizeInBits() const
    {
        return mySizeInBits;
    }

    static RecordUniqueId MakeUniqueId(const std::string& fileName, uint32_t fileOffset, const std::string& recordName);

    void InitUniqueId(const std::string& recordName) const;

    const FieldDecl* GetField(VirtualOffset offset) const;

    const FieldDecl* GetField(std::string_view fieldName) const;

    RecordType GetRecordType() const
    {
        return myRecordType;
    }

    const std::vector<SourceRange>& GetCommentBlocks() const
    {
        return myCommentBlocks;
    }
    // LCOV_EXCL_START
    std::string Dump() const override
    {
        return Node::Dump();
    }
    void Print(std::string& source, int entryOffset) const override
    {
        auto t = GetType();
        std::string name;
        switch (myRecordType) {
            case RecordType::STRUCT:
                name = "struct";
                break;
            case RecordType::UNION:
                name = "union";
                break;
            default:
                name = "class";
                break;
        }
        SafePrint(source, entryOffset, name + " " + t.GetPrintableName(), GetRange());
        for (auto field : myFields) {
            field->Print(source, entryOffset);
        }
        Base::Print(source, entryOffset);
    }
    // LCOV_EXCL_STOP

private:
    std::vector<NodePtr<FieldDecl>> myFields;
    RecordType myRecordType = RecordType::UNKNOWN;
    mutable RecordUniqueId myUniqueId = 0;
    bool myIsFirstDeclaration = false;
    bool myIsImplicit = false;
    uint64_t mySizeInBits = 0;
    std::vector<SourceRange> myCommentBlocks;
    bool myIsSensitive;
};

class CxxRecordDecl : public RecordDecl {
public:
    using BaseRecords = std::vector<NodePtr<CxxRecordDecl>>;
    using Methods = std::vector<NodePtr<CxxMethodDecl>>;

    CxxRecordDecl(const RecordDecl&& base, CxxDtorDecl* destructor, const std::vector<const CxxCtorDecl*>& constructors,
                  BaseRecords&& baseRecords)
        : RecordDecl(base),
          myDestructor(destructor),
          myConstructors(constructors),
          myBaseRecords(std::move(baseRecords))
    {}

    DECLARE_KIND(RecordDecl, Node::Kind::CXX_RECORD_DECL);
    DECLARE_SERIALIZE(CxxRecordDecl, myDestructor << myConstructors << myBaseRecords << myMethods);

    bool HasDestructor() const
    {
        return myDestructor != nullptr;
    }

    const CxxCtorDecl* FindDefaultCtor() const;

    /// The method returns the latest definition for each ctor
    const std::vector<const CxxCtorDecl*>& GetConstructors() const
    {
        return myConstructors;
    }

    const BaseRecords& GetBaseRecords() const
    {
        return myBaseRecords;
    }

    void SetMethods(Methods&& methods);

    const CxxMethodDecl* GetMethod(std::string_view methodName) const;

    // LCOV_EXCL_START
    void TraverseChildren(TraverseCallback callback) const override
    {
        CALL_CALLBACK(myDestructor, callback);
        RecordDecl::TraverseChildren(callback);
        for (auto& node : myConstructors) {
            CALL_CALLBACK(node, callback);
        }
    }
    // LCOV_EXCL_STOP

    void Clear() override
    {
        RecordDecl::Clear();
        myDestructor = nullptr;
        myConstructors.clear();
    }

private:
    const CxxDtorDecl* myDestructor = nullptr;
    std::vector<const CxxCtorDecl*> myConstructors;
    BaseRecords myBaseRecords;
    Methods myMethods;
};

}  // namespace HCXX

#endif  // COODDY_CXXRECORDDECL_H
