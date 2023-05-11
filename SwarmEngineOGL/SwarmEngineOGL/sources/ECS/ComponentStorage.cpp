#include "ECS/Components/ComponentStorage.hpp"

ComponentStorage &ComponentStorage::Get() {
    static ComponentStorage instance;
    return instance;
}
