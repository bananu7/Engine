#pragma once
#include <map>
#include <boost/optional.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <unordered_map>

namespace engine {
 
std::vector<unsigned char> buffer_from_file(std::istream& stream);

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class ResourceCache {
    struct base { virtual ~base() {} };
    template<typename T> struct derived : public base{
        T resource;
        derived(T t)
            : resource(std::move(t)) 
        {}
    };
    std::unordered_map<std::string, std::unique_ptr<base>> models;
public:
    template<typename T>
    T* Get(std::string name) {
        if (models.find(name) != models.end()) {
            if (auto p = dynamic_cast<derived<T>*>(models[name].get())) {
                return &p->resource;
            }
        }
        return nullptr;
    }
    template<typename T, typename Range>
    T* Get(std::string name, Range r) {
        auto p = Get<T>(name); if (p) return p;
        return &(models[name] = make_unique<derived<T>>(T::Load(r)))->resource;
    }
    void Unload(std::string name) {
        models.erase(name);
    }
};

/*
class ResourceCache {
    struct base { virtual ~base() {} };
    template<typename T> struct derived : public base {
        T resource;
        derived(T t)
            : t(std::move(resource)) 
        {}
    };
    std::unordered_map<std::string, std::unique_ptr<base>> models;
public:
    template<typename T>
    T* Get(std::string name) {
        if (models.find(name) != models.end()) {
            if (auto p = dynamic_cast<derived<T>*>(models[name].get())) {
                return &p->resource;
            }
        }
        return nullptr;
    }
    template<typename T, typename... Args>
    T* Get(std::string name, Args&&... args) {
        auto p = Get(name); 
        if (p)
            return p;
        return &(models[name] = make_unique<derived<T>>(T::Load(std::forward<Args>(args)...)))->resource;
    }
    void Unload(std::string name) {
        models.erase(name);
    }
};*/

} // namespace engine
