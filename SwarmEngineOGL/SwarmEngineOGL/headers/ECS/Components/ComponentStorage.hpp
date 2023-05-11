#pragma once

#include <map>
#include "ECSComponent.hpp"
#include "DynamicCollection.hpp"

class ComponentListBase
{
};

template <DerivedFromComponent ComponentType>
class ComponentList : public ComponentListBase, public DynamicCollection<ComponentType>
{
};

class ComponentStorage
{
public:
    static ComponentStorage& Get();

    template <DerivedFromComponent ComponentType>
    ComponentType* CreateComponent(ComponentType&& component)
    {
        auto& componentList = GetComponentList<ComponentType>();
        return componentList.Add(std::forward<ComponentType>(component));
    }

private:
    std::map<uint64_t, ComponentListBase*> _components{};   // todo unique_ptr

private:
    template <DerivedFromComponent ComponentType>
    ComponentList<ComponentType>& GetComponentList()
    {
        const auto componentTypeID = typeid(ComponentType).hash_code();

        if (!_components.contains(componentTypeID))
        {
            _components[componentTypeID] = new ComponentList<ComponentType>();
        }

        return *static_cast<ComponentList<ComponentType>*>(_components[componentTypeID]);
    }
};
