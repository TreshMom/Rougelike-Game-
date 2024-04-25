#pragma once
#include <type_traits>

template<class ...Types>
struct List {};

template<class Type, class ...Types>
struct List<Type, Types...> {
    using head_ = Type;
    using tail_ = List<Types...>;
};

template<class ...Components>
struct ListToTuple;

template<class ...Components>
struct ListToTuple<List<Components...>>
{
    using type = std::tuple<Components...>;
};

template<class T>
using ListToTuple_t = ListToTuple<T>::type;

template<class ...Types>
struct FoundType {};        

template<class Component>
struct FoundType<Component, List<>> {
    const static bool value = false;
};

template<class Component, class EC>
struct FoundType<Component, EC> {
    const static bool value = std::is_same_v<Component, typename EC::head_> 
    || FoundType<Component, typename EC::tail_>::value;
};

template<class... Types>
struct concate;

template<class... Types1, class... Types2>
struct concate<List<Types1...>,List<Types2...>>
{
    using type = List<Types1...,Types2...>;
};

template<class T1, class T2>
using concate_t = concate<T1,T2>::type;

template<class... Types>
struct for_earch_with_concate;

template<>
struct for_earch_with_concate<List<>>
{
    using type = List<>;
};

template<class Type, class... Types>
struct for_earch_with_concate<List<Type, Types...>>
{
    using type = concate_t<List<ListToTuple_t<typename Type::tail_::head_>>, 
    typename for_earch_with_concate<List<Types...>>::type>;
};

template<class... Types>
struct ListToVariant;

template<class... Types>
struct ListToVariant<List<Types...>>
{
    using type = std::variant<Types...>;
};

template<class T>
using ListToVariant_t = ListToVariant<T>::type;


template <typename T, typename Tuple>
struct has_type;

template <typename T>
struct has_type<T, std::tuple<>> {
    constexpr static bool value = false;
};

template <typename T, typename U, typename... Ts>
struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {};

template <typename T, typename... Ts>
struct has_type<T, std::tuple<T, Ts...>> {
    constexpr static bool value = true;
};

// maybe
template<class T>
struct Just
{
    using type = T;
};

struct Nothing
{};

template<class...T>
struct MayBe;

template<class T>
struct MayBe<T> : Just<T>
{};

template<>
struct MayBe<> : Nothing
{};

// Maybe a -> bool
template<class...args>
struct MayBeToBool;

template<class T>
struct MayBeToBool<MayBe<T>>
{
    constexpr static bool value = true;
};

template<>
struct MayBeToBool<MayBe<>>
{
    constexpr static bool value = false;
};

// M a -> (a -> bool) -> Maybe a
template<class ListType, template<typename> class FuncType, class... a>
struct find;

// [a] -> (a -> bool) -> Maybe a
template<template<typename> class FuncType>
struct find<List<>, FuncType>
{
    using type = MayBe<>; 
};

template<template<typename> class FuncType, class head, class... tail>
struct find<List<head, tail...>, FuncType>
{
    using type = std::conditional_t<FuncType<head>::value, MayBe<head>, typename find<List<tail...>, FuncType>::type>;
};

template<class ListType, template<typename> class FuncType, class... a>
using find_t = typename find<ListType, FuncType, a...>::type ;

template<class Type, class Other, class...T>
struct is_equal_head
{
    constexpr static bool value = false; 
};

template<class Type>
struct is_equal_head<Type, List<>>
{
    constexpr static bool value = false; 
};

template<class Type, class a, class...args>
struct is_equal_head<Type, List<a, args...>>
{
    constexpr static bool value = std::is_same_v<Type,a>; 
};

template<template <typename...> class TemplateFunction,class Type>
struct curry_two_impl_end
{
    template<class... Types>
    using type =  TemplateFunction<Type,Types...>;
};


template<template <typename...> class TemplateFunction, class... Types>
concept is_invocable_type = requires{TemplateFunction<Types...>::value;};

template<template <typename...> class TemplateFunction, class... OtherTypes>
struct curry_impl;

template<template <typename...> class TemplateFunction, class... OtherTypes> 
requires (is_invocable_type<TemplateFunction,OtherTypes...>)
struct curry_impl<TemplateFunction, OtherTypes...>
{
    constexpr static bool value = TemplateFunction<OtherTypes...>::value;
};

template<template <typename...> class TemplateFunction, class... OtherTypes>
requires (!is_invocable_type<TemplateFunction,OtherTypes...>)
struct curry_impl<TemplateFunction, OtherTypes...>
{
    template<class Type>
    using type = curry_impl<TemplateFunction, OtherTypes...,Type>;

};

template<template <typename...> class TemplateFunction>
using curry = curry_impl<TemplateFunction>;
