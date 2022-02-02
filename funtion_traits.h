#pragma once

#include <functional>

template<typename T> struct function_traits;

template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
{
    enum { arity = sizeof...(Args) };
    typedef Ret return_type;
    typedef std::tuple<Args...> tuple_type;
    typedef std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...> base_type;
    typedef Ret function_type(Args...);
    using stl_function_type = std::function<function_type>;
};

template <typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};

template <typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>>
    : function_traits<Ret(Args...)> {};

template <typename ReturnType, typename ClassType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...)>
    : function_traits<ReturnType(Args...)> {};

template <typename ReturnType, typename ClassType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
    : function_traits<ReturnType(Args...)> {};

template <typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())> {};

template <typename T>
using remove_const_reference_t =
std::remove_const_t<std::remove_reference_t<T>>;