#pragma once
#include <vector>
#include <functional>

namespace Ath::Control
{
    #define MEMBER_FUNCTION(instance, method) std::bind(method, std::ref(instance), std::placeholders::_1) 

    #define addMemberCallback(instance, method) addCallback(MEMBER_FUNCTION(instance, method))

    template <typename T>
    class EventOutput
    {   
        typedef std::function<void(T)> callbackType;

        std::vector<callbackType> callbacks;

        public:
            void fire(const T value) 
            {
                for(callbackType callback : callbacks)    callback(value);
            }

            void addCallback(callbackType callback) { callbacks.push_back(callback); }
    };
}