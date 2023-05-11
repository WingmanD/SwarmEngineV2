#pragma once


#include <algorithm>
#include <string>
#include <typeinfo>
#include <vector>
#include <iostream>

class Type
{
public:
    Type() = default;

    Type(uint64_t id, std::string name);

    [[nodiscard]] uint64_t GetId() const;

    [[nodiscard]] const std::string& GetName() const;

    [[nodiscard]] std::string GetFullName() const;

    [[nodiscard]] const std::vector<Type>& GetParents() const;

    [[nodiscard]] const std::vector<Type>& GetCompositeTypes() const;

    template <typename T, typename... CompositeTypes>
    static Type MakeCompositeType()
    {
        Type newType(typeid(T).hash_code(), typeid(T).name());
        newType._compositeTypes = {MakeCompositeType<CompositeTypes>()...};

        return newType;
    }

    template <typename T>
    [[nodiscard]] bool HasA() const
    {
        return std::ranges::any_of(_compositeTypes, [](const auto& compositeType)
        {
            return compositeType.GetId() == typeid(T).hash_code();
        });
    }

    template <typename... T>
    [[nodiscard]] bool HasAll() const
    {
        return (HasA<T>() && ...);
    }

    template <typename... T>
    [[nodiscard]] bool Matches() const
    {
        return (HasA<T>() && ...) && (sizeof...(T) == _compositeTypes.size());
    }

    friend bool operator==(const Type& lhs, const Type& rhs);
    friend std::strong_ordering operator<=>(const Type& lhs, const Type& rhs);

private:
    uint64_t _id = 0;
    std::string _name;

    std::vector<Type> _parents;
    std::vector<Type> _compositeTypes;
};
