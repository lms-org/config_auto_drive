#ifndef SENSOR_ENVIRONMENT_H
#define SENSOR_ENVIRONMENT_H
#include <vector>
#include <memory>
namespace street_environment {

class EnvironmentObject{
public:
    template <typename T>
    std::shared_ptr<T> getCopyAsPtr() const{
        return std::shared_ptr<T>(new T(*static_cast<const T*>(this)));
    }

    template <typename T>
    T& getAsReference() const{
        return *static_cast<T*>(this);
    }


};

class Environment{
public:
    std::vector<std::shared_ptr<EnvironmentObject>> objects;
};
}


#endif /* SENSOR_ENVIRONMENT_H */
