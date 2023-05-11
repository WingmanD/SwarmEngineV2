#pragma once

#include <utility>
#include <functional>
#include <array>

template <typename T, size_t size = 500>
class FixedCollection
{
public:
    FixedCollection() = default;

    FixedCollection(const FixedCollection&) = delete;

    FixedCollection(FixedCollection&&) = delete;

    FixedCollection& operator=(const FixedCollection&) = delete;

    FixedCollection& operator=(FixedCollection&&) = delete;

    [[nodiscard]] bool IsFull() const
    {
        return _currentIndex >= _elements.size() && _deletedIndices.empty();
    }

    T* Add(T&& item)
    {
        if (IsFull())
        {
            return nullptr;
        }

        if (!_deletedIndices.empty())
        {
            auto index = _deletedIndices.back();
            _deletedIndices.pop_back();

            _elements[index] = std::forward<T>(item);

            return &_elements[index];
        }

        _elements[_currentIndex] = std::forward<T>(item);
        return &_elements[_currentIndex++];
    }

    bool Contains(T* item)
    {
        auto index = item - _elements.data();

        return index >= 0 && index < _elements.size();
    }

    void Remove(T* item)
    {
        auto index = item - _elements.data();

        if (index < 0 || index >= _elements.size())
        {
            // todo log
            return;
        }

        _deletedIndices.push_back(index);
    }

    void ForEach(std::function<void(T&)> func)
    {
        constexpr bool IsValidateable = requires(const T& t)
        {
            t.IsValid();
        };

        for (auto& item : _elements)
        {
            if constexpr (IsValidateable)
            {
                if (!item.IsValid())
                {
                    continue;
                }
            }

            func(item);
        }
    }

private:
    size_t _currentIndex = 0;
    std::array<T, size> _elements;

    std::vector<size_t> _deletedIndices;
};
