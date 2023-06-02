/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef NAMED_NODE_HEADER_FILE
#define NAMED_NODE_HEADER_FILE

#include <ast/Node.h>

namespace HCXX {

template <typename BaseClass>
class NamedNode : public BaseClass {
public:
    std::string GetFQN() const override
    {
        return myQualifier != nullptr ? myQualifier->GetFQN() + "::" + myName : myName;
    }

    const std::string& GetName() const override
    {
        return myName;
    }
    void SetName(std::string&& name)
    {
        myName = name;
    }

    const Node* GetQualifier() const
    {
        return myQualifier;
    }
    // LCOV_EXCL_START
    std::optional<std::string> GetAttribute(std::string_view attrName) const override
    {
        if (attrName == "Name") {
            return myName;
        }

        return BaseClass::GetAttribute(attrName);
    }
    // LCOV_EXCL_STOP

protected:
    NamedNode(){};
    ~NamedNode() = default;

    template <class... Args>
    NamedNode(Node* qualifier, std::string&& name, Args... args)
        : BaseClass(std::forward<Args>(args)...), myName(std::move(name)), myQualifier(qualifier)
    {}

    template <class... Args>
    NamedNode(Node* qualifier, std::string_view name, Args... args)
        : BaseClass(std::forward<Args>(args)...), myName(name), myQualifier(qualifier)
    {}

    template <class... Args>
    NamedNode(Node* qualifier, const char* name, Args... args)
        : BaseClass(std::forward<Args>(args)...), myName(name), myQualifier(qualifier)
    {}

private:
    std::string myName;
    /** Semantic parent node that "owns" current node and thus responsible for it's FQN prefix */
    const Node* myQualifier = nullptr;
};

}  //  namespace HCXX

#endif  // #ifndef NAMED_NODE_HEADER_FILE
