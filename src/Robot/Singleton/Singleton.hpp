#ifndef ROBOT_SINGLETON_HPP
#define ROBOT_SINGLETON_HPP

#include "./DataTransmissions/TcpSession/TcpSession.hpp"

template<typename Type>
concept is_default_constructor = requires(Type obj){ obj(); };

template<typename Type>
concept is_singleton = !is_default_constructor<Type>;

template<is_singleton Type>
class Singleton {
private:
    Type *obj;

    Singleton() { obj = new Type; }
    ~Singleton() { delete obj;}

public:

    Type& GetInstance(){
        static Type singleton;
        return *singleton.obj;
    }

    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
};

///extern Singleton<lib::network::TcpSession>;
#endif
