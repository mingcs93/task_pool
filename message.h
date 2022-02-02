#pragma once

#include "funtion_traits.h"
#include <tuple>
#include <memory>

class MessageBase
{
public:
    MessageBase(const uint64_t cmdID)
        : m_cmdID(cmdID)
    {}

    virtual uint64_t get_cmd() const { return m_cmdID; }

private:
    uint64_t m_cmdID;
};

template<typename... Args>
class Message : public MessageBase
{
public:
    /*Message(const uint64_t cmdID, Args&&... args)
        : MessageBase(cmdID)
        , m_params(std::forward_as_tuple(args...))
    {}*/

    Message(const uint64_t cmdID, const std::tuple<Args...>& tp)
        : MessageBase(cmdID)
        , m_params(tp)
    {}

    Message(const uint64_t cmdID, std::tuple<Args...>&& tp)
        : MessageBase(cmdID)
        , m_params(std::move(tp))
    {}

    const std::tuple<Args...>& get_params() const { return m_params; }

private:
    std::tuple<Args...> m_params;
};

template<typename... Args>
decltype(auto)  make_message(const uint64_t cmdID, Args&&... args)
{
    /*auto r =  std::make_shared<Message<Args...>>(cmdID, std::forward_as_tuple(args...));
    std::cout << typeid(*r).name() << std::endl;*/
    return std::make_shared<Message<Args...>>(cmdID, std::forward_as_tuple(args...));
}

template<typename... Args>
decltype(auto)  make_message(const uint64_t cmdID, const Args&... args)
{
    /*auto r =  std::make_shared<Message<Args...>>(cmdID, std::forward_as_tuple(args...));
    std::cout << typeid(*r).name() << std::endl;*/
    return std::make_shared<Message<Args...>>(cmdID, std::forward_as_tuple(args...));
}


template<typename... Args>
decltype(auto)  make_message_as_tuple(const uint64_t cmdID, const std::tuple<Args...>& tp)
{
    /* auto r = std::make_shared<Message<Args...>>(cmdID, std::move(tp));
     std::cout << typeid(*r).name() << std::endl;*/
    return std::make_shared<Message<Args...>>(cmdID, std::move(tp));
}