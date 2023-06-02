/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_INCLUDE_ANNOTATION_H_
#define COODDY_ANALYZER_INCLUDE_ANNOTATION_H_

#include <ast/Node.h>
#include <ast/VirtualOffset.h>
#include <utils/EnumMapping.h>

#include <cstdint>
#include <cstring>
#include <mutex>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace HCXX {

class Annotation {
public:
    struct ArgInfo {
        static const uint16_t MAX_ARG_POS = 0xFF;
        static const uint16_t MAX_INDEX = 0xFFFF;
        explicit ArgInfo(uint32_t argPos = MAX_ARG_POS, uint32_t indexInArgAnnotations = MAX_INDEX,
                         bool isSourceRange = true, uint32_t indexOfOverridden = 0)
            : argPos(argPos),
              indexInArgAnnotations(indexInArgAnnotations),
              isSourceRange(isSourceRange),
              indexOfOverridden(indexOfOverridden)
        {}
        uint16_t argPos : 8;             // argument position in the callee function context (if not MAX_ARG_POS)
        uint16_t isSourceRange : 1;      // equals 1 if annotation was set inside current function
        uint16_t indexOfOverridden : 7;  // index of overridden function context
        uint16_t indexInArgAnnotations;  // index in annotations of argument (if not MAX_INDEX)
    };

    using Kind = uint8_t;

    // clang-format off
    enum Trait : uint32_t {
        PROPAGATED_BY_DECL            = 0b1,
        PROPAGATED_BY_POINTER         = 0b10,
        JOINED_BY_AND                 = 0b100,
        NOT_PROPAGATED_THROUGH_LOOP   = 0b1000,
        UNTRUSTED_SINK                = 0b10000,
        PROPAGATED_BY_ALIAS           = 0b100000,
        NOT_PROPAGATED_TO_SUB_SCOPES  = 0b1000000,
        PROPAGATED_TO_FIELDS          = 0b10000000,
        NOT_PROPAGATED_FROM_PARAMETER = 0b100000000,
        ANNOTATE_CALL_INSTRUCTION     = 0b1000000000,
        NOT_PROPAGATED_BY_STATIC_VAR  = 0b10000000000,
    };
    // clang-format on

    static Kind RegisterKind(std::string_view kindName, uint32_t traits);

    static Kind GetKind(std::string_view kindName);

    static std::string_view GetKindName(Kind kind);

    // Returns true if annotation should be propagated via expression declaration
    static bool IsPropagatedByDecl(Annotation::Kind kind)
    {
        return myTraits[kind] & PROPAGATED_BY_DECL;
    }

    // Returns true if annotation should be propagated through function argument passed by pointer
    static bool IsPropagatedByPointer(Annotation::Kind kind)
    {
        return myTraits[kind] & (PROPAGATED_BY_DECL | PROPAGATED_BY_POINTER);
    }

    // Returns true if annotation should be joined by And operation
    static bool IsJoinedByAnd(Annotation::Kind kind)
    {
        return myTraits[kind] & JOINED_BY_AND;
    }

    // Returns true if annotation should be propagated through loops
    static bool IsPropagatedThroughLoop(Annotation::Kind kind)
    {
        return !(myTraits[kind] & NOT_PROPAGATED_THROUGH_LOOP);
    }

    static bool IsUntrustedSink(Annotation::Kind kind)
    {
        return myTraits[kind] & UNTRUSTED_SINK;
    }

    static bool IsPropagatedByAlias(Annotation::Kind kind)
    {
        return myTraits[kind] & PROPAGATED_BY_ALIAS;
    }

    static bool IsPropagatedToSubScopes(Annotation::Kind kind)
    {
        return !(myTraits[kind] & NOT_PROPAGATED_TO_SUB_SCOPES);
    }

    static bool IsPropagatedToFields(Annotation::Kind kind)
    {
        return myTraits[kind] & PROPAGATED_TO_FIELDS;
    }

    /// If true, this annotation does not transfer to call arguments when a parameter is marked with it.
    static bool IsPropagatedFromParameter(Annotation::Kind kind)
    {
        return !(myTraits[kind] & NOT_PROPAGATED_FROM_PARAMETER);
    }

    /// If true, this annotation is set to the whole call instruct instead of call argument.
    static bool AnnotateCallInstruction(Annotation::Kind kind)
    {
        return (myTraits[kind] & ANNOTATE_CALL_INSTRUCTION);
    }

    /// If true, this annotation can be propagated via static variables
    static bool IsPropagatedByStaticVar(Annotation::Kind kind)
    {
        return !(myTraits[kind] & NOT_PROPAGATED_BY_STATIC_VAR);
    }

    static uint32_t GetAnnotationsCount()
    {
        return myTraits.size();
    }

    bool operator==(const Annotation& other) const
    {
        return myKind == other.myKind && myArgInfo.argPos == other.myArgInfo.argPos &&
               myArgInfo.indexOfOverridden == other.myArgInfo.indexOfOverridden &&
               myInstruction == other.myInstruction && myMemoryOffset == other.myMemoryOffset &&
               myUserData == other.myUserData;
    }

    Annotation(Kind kind = 0, ArgInfo argInfo = ArgInfo(), uint32_t instruction = 0,
               VirtualOffset memoryOffset = VirtualOffset(), uint64_t userData = 0)
        : myKind(static_cast<uint8_t>(kind)),
          myInstruction(instruction),
          myArgInfo(argInfo),
          myMemoryOffset(memoryOffset),
          myUserData(userData)
    {}
    // TODO HCAT-661
    // COODDY_SUPPRESS InitFieldsInConstructorsChecker
    Annotation(const Annotation& source, VirtualOffset memoryOffset) : Annotation(source)
    {
        myMemoryOffset = memoryOffset;
    }

    ~Annotation() = default;

    Kind GetKind() const
    {
        return static_cast<Kind>(myKind);
    }
    void SetKind(Annotation::Kind kind)
    {
        myKind = kind;
    }
    // returns true if annotation was set inside current function
    bool IsSourceRange() const
    {
        return myArgInfo.isSourceRange;
    }
    /// returns true if annotation came from a return value
    bool IsRetValueSource() const
    {
        return GetArgPos() == 0;
    }

    uint32_t GetArgPos() const
    {
        return myArgInfo.argPos;
    }

    uint32_t GetIndexInArgAnnotations() const
    {
        return myArgInfo.indexInArgAnnotations;
    }

    ArgInfo GetArgInfo() const
    {
        return myArgInfo;
    }

    uint32_t GetInstruction() const
    {
        return myInstruction;
    }
    VirtualOffset GetMemoryOffset() const
    {
        return myMemoryOffset;
    }
    // returns true if annotation belongs to the whole object (not to offset inside of object)
    bool IsSourceObject() const
    {
        return myMemoryOffset.IsSourceObject();
    }

    /**
     * @brief Annotation's user-defined data.
     *
     * Data is stored internally as a single uint64_t. This function reinterprets the data as any type.
     *
     * @tparam T Type which 8-byte user data is interpreted as.
     */
    template <class T = uint64_t>
    T& GetUserData() const
    {
        static_assert(sizeof(T) <= sizeof(uint64_t), "The size of user data type is too big");
        // COODDY_SUPPRESS
        return *(T*)&myUserData;
    }

    static const uint32_t MAX_ANNOTATIONS_COUNT = 64;

private:
    static std::vector<uint32_t> myTraits;
    static std::unordered_map<std::string_view, Kind> myKinds;
    static std::unordered_set<std::string> myKindsKeyStorage;  // Remove in C++20 by replacing map key with string
    static std::mutex myMutex;

    ArgInfo myArgInfo;
    uint32_t myKind : 8;           // kind of the annotation
    uint32_t myInstruction : 24;   // instruction in the function behaviour context
    VirtualOffset myMemoryOffset;  // annotation offset

    /// Arbitrary 8 bytes of user-defined data for the annotation
    uint64_t myUserData;
};

static_assert(sizeof(Annotation) <= 24, "The size of Annotation is too large");

using ParamAnnotation = std::vector<Annotation>;
using FunctionAnnotation = std::vector<ParamAnnotation>;

};  // namespace HCXX

#endif  // COODDY_ANALYZER_INCLUDE_ANNOTATION_H_
