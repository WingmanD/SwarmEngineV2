#include "Type.hpp"
#include <ranges>
#include <algorithm>
#include <numeric>

Type::Type(uint64_t id, std::string name) : _id(id), _name(std::move(name))
{
}

uint64_t Type::GetId() const
{
    return _id;
}

const std::string& Type::GetName() const
{
    return _name;
}

std::string Type::GetFullName() const
{
    std::string fullName = _name;

    if (!_parents.empty())
    {
        fullName += "{";

        for (const auto& parent : _parents)
        {
            fullName += parent.GetFullName();

            if (&parent != &_parents.back())
            {
                fullName += ", ";
            }
        }

        fullName += "}";
    }

    if (!_compositeTypes.empty())
    {
        fullName += "[";

        for (const auto& composite : _compositeTypes)
        {
            fullName += composite.GetFullName();

            if (&composite != &_compositeTypes.back())
            {
                fullName += ", ";
            }
        }

        fullName += "]";
    }

    return fullName;
}

const std::vector<Type>& Type::GetParents() const
{
    return _parents;
}

const std::vector<Type>& Type::GetCompositeTypes() const
{
    return _compositeTypes;
}

std::strong_ordering operator<=>(const Type& lhs, const Type& rhs)
{
    if (lhs._id != rhs._id)
    {
        return lhs._id <=> rhs._id;
    }

    if (lhs._compositeTypes.size() != rhs._compositeTypes.size())
    {
        return lhs._compositeTypes.size() <=> rhs._compositeTypes.size();
    }

    if (lhs._parents.size() != rhs._parents.size())
    {
        return lhs._parents.size() <=> rhs._parents.size();
    }

    const auto sumCompositeLeft = std::accumulate(lhs._compositeTypes.begin(), lhs._compositeTypes.end(), 0,
                                                  [](auto sum, const auto& type)
                                                  {
                                                      return sum + type.GetId();
                                                  });

    const auto sumCompositeRight = std::accumulate(rhs._compositeTypes.begin(), rhs._compositeTypes.end(), 0,
                                                   [](auto sum, const auto& type)
                                                   {
                                                       return sum + type.GetId();
                                                   });

    if (sumCompositeLeft != sumCompositeRight)
    {
        return sumCompositeLeft <=> sumCompositeRight;
    }

    const auto sumParentsLeft = std::accumulate(lhs._parents.begin(), lhs._parents.end(), 0,
                                                [](auto sum, const auto& type)
                                                {
                                                    return sum + type.GetId();
                                                });

    const auto sumParentsRight = std::accumulate(rhs._parents.begin(), rhs._parents.end(), 0,
                                                 [](auto sum, const auto& type)
                                                 {
                                                     return sum + type.GetId();
                                                 });

    return sumParentsLeft <=> sumParentsRight;
}

bool operator==(const Type& lhs, const Type& rhs)
{
    if (lhs._id == rhs._id)
    {
        return true;
    }

    if (lhs._compositeTypes.size() != rhs._compositeTypes.size())
    {
        return false;
    }

    if (lhs._parents.size() != rhs._parents.size())
    {
        return false;
    }

    const auto sumCompositeLeft = std::accumulate(lhs._compositeTypes.begin(), lhs._compositeTypes.end(), 0,
                                                  [](auto sum, const auto& type)
                                                  {
                                                      return sum + type.GetId();
                                                  });
    const auto sumCompositeRight = std::accumulate(rhs._compositeTypes.begin(), rhs._compositeTypes.end(), 0,
                                                   [](auto sum, const auto& type)
                                                   {
                                                       return sum + type.GetId();
                                                   });

    if (sumCompositeLeft != sumCompositeRight)
    {
        return false;
    }

    const auto sumParentsLeft = std::accumulate(lhs._parents.begin(), lhs._parents.end(), 0,
                                                [](auto sum, const auto& type)
                                                {
                                                    return sum + type.GetId();
                                                });

    const auto sumParentsRight = std::accumulate(rhs._parents.begin(), rhs._parents.end(), 0,
                                                 [](auto sum, const auto& type)
                                                 {
                                                     return sum + type.GetId();
                                                 });

    return sumParentsLeft == sumParentsRight;
}
