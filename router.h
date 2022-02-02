#pragma once

#include "message.h"
#include <stdexcept>
#include <memory>
#include <map>
#include <typeinfo>

class Router
{
public:
    template<typename Function>
    void register_handler(const uint64_t cmdID, const Function& func)
    {
        m_invokers[cmdID] = std::bind(Invoker<Function>::apply, std::move(func), std::placeholders::_1);
    }

    template<typename Function, typename Self>
    void register_handler(const uint64_t cmdID, const Function& func, const std::shared_ptr<Self> self)
    {
        m_invokers[cmdID] = std::bind(Invoker<Function>::template apply_member<Self>, std::move(func), self, std::placeholders::_1);
    }

    void dispach(const std::shared_ptr<MessageBase> msg)
    {
        if (msg == nullptr)
        {
            throw std::invalid_argument("msg == nullptr");
        }

        auto iter = m_invokers.find(msg->get_cmd());
        if (iter == m_invokers.cend())
        {
            throw std::invalid_argument("m_invokers.find(msg->get_cmd()) == m_invokers.cend()");
        }
        iter->second(msg);
    }

private:
    template<typename Function>
    struct Invoker
    {
        static inline void apply(const Function& func, const std::shared_ptr<MessageBase> msg)
        {
            using args_type = typename function_traits<Function>::base_type;
            using msg_type = decltype(Message{ std::declval<uint64_t>(), std::declval<args_type>() });
            std::shared_ptr<msg_type> real_msg = std::dynamic_pointer_cast<msg_type, MessageBase>(msg);
            if (real_msg == nullptr)
            {
                throw std::invalid_argument("real_msg == nullptr");
            }
            auto params = real_msg->get_params();
            call(func, std::move(params));
        }

        template<typename Self>
        static inline void apply_member(const Function& func, const std::shared_ptr<Self> self, const std::shared_ptr<MessageBase> msg)
        {
            using args_type = typename function_traits<Function>::base_type;
            using msg_type = decltype(Message{ std::declval<uint64_t>(), std::declval<args_type>() });
            //std::cout << typeid(Message{ uint64_t{}, args_type{} }).name() << std::endl;
            std::shared_ptr<msg_type> real_msg = std::dynamic_pointer_cast<msg_type, MessageBase>(msg);
            if (real_msg == nullptr)
            {
                throw std::invalid_argument("real_msg == nullptr");
            }
            auto params = real_msg->get_params();
            call_member(func, self, params);
        }
    };

    template<typename Function, typename... Args>
    static inline void call(const Function& func, const std::tuple<Args...>& args)
    {
        call_helper(func, std::make_index_sequence<sizeof...(Args)>{}, std::move(args));
    }

    template<typename Function, typename Self, typename... Args>
    static inline void call_member(const Function& func, const std::shared_ptr<Self> self, const std::tuple<Args...>& args)
    {
        call_member_helper(func, self, std::make_index_sequence<sizeof...(Args)>{}, std::move(args));
    }

    template<typename Function, size_t... I, typename... Args>
    static inline void call_helper(const Function& func, const std::index_sequence<I...>&, const std::tuple<Args...>& args)
    {
        func(std::move(std::get<I>(args))...);
    }

    template<typename Function, typename Self, size_t... I, typename... Args>
    static inline void call_member_helper(const Function& func, const std::shared_ptr<Self> self, const std::index_sequence<I...>&, const std::tuple<Args...>& args)
    {
        (*self.*func)(std::move(std::get<I>(args))...);
    }

private:
    std::map<uint64_t, std::function<void(const std::shared_ptr<MessageBase>)>> m_invokers;
};