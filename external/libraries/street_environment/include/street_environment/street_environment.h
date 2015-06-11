#ifndef SENSOR_ENVIRONMENT_H
#define SENSOR_ENVIRONMENT_H
#include <vector>
#include <memory>
namespace street_environment {

class EnvironmentObject{
private:
    std::string m_name;
public:
    template <typename T>
    std::shared_ptr<T> getCopyAsPtr() const{
        return std::shared_ptr<T>(new T(*static_cast<const T*>(this)));
    }

    template <typename T>
    T& getAsReference() const{
        return *static_cast<T*>(this);
    }

    std::string name(){
        return m_name;
    }
    void name(std::string name){
        m_name = name;
    }


};

class Environment{

    std::shared_ptr<EnvironmentObject> getObjectByName(std::string name){
        for(std::shared_ptr<EnvironmentObject> &o: objects){
            if(o->name() == name){
                return o;
            }
        }
        return nullptr;
    }

public:
    std::vector<std::shared_ptr<EnvironmentObject>> objects;
};
}


#endif /* SENSOR_ENVIRONMENT_H */
