#pragma once

#include <memory>
#include "FixedCollection.hpp"

template<typename T>
class DynamicCollection {
public:
    T *Add(T &&item) {
        for (auto &collection: _collections) {
            if (!collection->IsFull()) {
                return collection->Add(std::forward<T>(item));
            }
        }

        auto newCollection = std::make_unique<FixedCollection<T>>();
        auto newCollectionPtr = newCollection.get();
        _collections.push_back(std::move(newCollection));

        return newCollectionPtr->Add(std::forward<T>(item));
    }

    void ForEach(std::function<void(T &)> func) {
        for (auto &collection: _collections) {
            collection->ForEach(func);
        }
    }

private:
    std::vector<std::unique_ptr<FixedCollection<T>>> _collections;
};
