#pragma once

#include <stdlib.h>       // for atexit
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::milliseconds
#include <mutex>          // std::call_once, std::once_flag

#include <boost/noncopyable.hpp>

namespace gzqxm{
    namespace utils{
        template<typename T>
        class Singleton : boost::noncopyable
        {
            public:
                static T& instance(){
                    std::call_once(_once, init);
                    assert(_value != nullptr);
                    return *_value;
                }

            private:
                Singleton(); 
                ~Singleton();

                void init(){
                    _value = new T();
                    atexit(destroy);
                }

                static void destroy() {
                   delete _value;
                   _value = nullptr; 
                }

            private:
                static std::once_flag _once;
                static T*             _value; 
        } // class Singleton

        template<typename T>
        std::once_flag Singleton<T>::_once;

        template<typename T>
        T* Singleton<T>::_value = nullptr;
    }

    namespace extra{
        template <typename T>
        class Singleton {
        private:
            static T* volatile _instance;
            static std::mutex mu;
            static void Destroy() {
                if(_instance != nullptr) {
                    delete _instance;
                    _instance = nullptr;
                }
            }

            Singleton(){}

        public:
            static T & Instance() {
                if(_instance == nullptr) {
                    std::lock_guard<std::mutex> lock(mu);
                    _instance = new T;
                    atexit(Destroy);
                }
                return *_instance;
            }

            Singleton(const Singleton&) = delete;
            Singleton& operator =(const Singleton&) = delete;
            Singleton(Singleton&&) = delete;
            Singleton& operator = (Singleton&&) = delete;
        };

        template <typename T>
        T * volatile Singleton<T >::_instance = nullptr;

        template <typename T>
        std::mutex Singleton<T>::mu;
    }
}