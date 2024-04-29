#pragma once
#include <type_traits>


namespace utils {

}

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


///////////// CONCATE ///////////////
template<class... Ts>
struct concate;

template<class... Ts1, class... Ts2>
struct concate <List<Ts1...>, List<Ts2...>>{
    using type = List<Ts1..., Ts2...>;
};

template<class T1, class T2>
using concate_t = concate<T1, T2>::type;


/////////////// COMPOSITION ////////////////////////
template<template<typename> class... Fs>
struct composition;

template<template<typename> class Func>
struct composition<Func> {
    template<class T>
    using type = typename Func<T>::type;
};

template<template<typename> class Func1, template<typename> class... Func2>
struct composition<Func1, Func2...> {
    template<class T>
    using type = typename Func1<typename composition<Func2...>::template type<T>>::type;
};

////////////////////// MAP //////////////////
template<template<typename> class Func, class... Ts>
struct map;

template<template<typename> class Func>
struct map <Func, List<>> {
    using type = List<>;
};

template<template<typename> class Func, class T, class ...Ts>
struct map <Func, List<T, Ts...>> {
    using type = concate_t<List<typename Func<T>::type>, typename map<Func, List<Ts...>>::type>;
};

/////////////////////// MAYBE ////////////////////
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

//////////////////////// Filter /////////////////

// M a -> (a -> bool) -> M a
template<class ListType, template<typename> class FuncType, class... a>
struct filter;

template<template<typename> class FuncType>
struct filter<List<>, FuncType> {
    using type = List<>; 
};

template<template<typename> class FuncType, class head, class... tail>
struct filter<List<head, tail...>, FuncType> {
    using type = std::conditional_t<FuncType<head>::value, concate_t<List<head>, typename filter<List<tail...>, FuncType>::type>, 
    typename filter<List<tail...>, FuncType>::type >;
};

/////////////////////// find ////////////////////

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

template<template<typename> class FuncType>
struct find<std::tuple<>, FuncType>
{
    using type = MayBe<>; 
};

template<template<typename> class FuncType, class head, class... tail>
struct find<std::tuple<head, tail...>, FuncType>
{
    using type = std::conditional_t<FuncType<head>::value, MayBe<head>, typename find<std::tuple<tail...>, FuncType>::type>;
};

template<class ListType, template<typename> class FuncType, class... a>
using find_t = typename find<ListType, FuncType, a...>::type ;

//////////////////////// CURRY /////////////////

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

///////////////////////// unique ///////////////

template<class Left, class Right>
struct is_no_same
{
    constexpr static bool value = !std::is_same_v<Left,Right>;
};

template<class... Types>
struct unique;

template<>
struct unique<List<>>
{
    using type = List<>;
};

template<class Type, class... Types>
struct unique<List<Type, Types...>>
{
    using type = concate_t<List<Type>, typename unique<typename filter<List<Types...>, curry<is_no_same>::template type<Type>::template type>::type>::type>;
};


/////////////////////// SOME SHIT ///////////////

template<class T>
struct get_tail
{
    using type = typename T::tail_;
};

template<class T>
struct get_head
{
    using type = typename T::head_;
};

template<class T>
struct to_list
{
    using type = List<T>;
};

template<class T>
struct Identity
{
    using type = T;
};

template<class T>
struct to_ptr
{
    using type = std::shared_ptr<T>;
};

template<class ListType>
struct list_types_to_pointers
{
    using type = typename map<to_ptr, ListType>::type;
};

template<class T>
struct tail_to_pointer_to_tuple {
    using type = typename composition<ListToTuple, list_types_to_pointers , get_head, get_tail>::type<T>;
};

template<class ListType>
struct for_each_with_concate_tail {
    using type = typename unique<typename map<tail_to_pointer_to_tuple, ListType>::type>::type;
};

template<class T>
struct head_to_pointer {
     using type = typename composition<to_ptr, get_head>::type<T>;
};

template<class ListType>
struct for_each_with_concate_head {
    using type = typename map<head_to_pointer, ListType>::type;
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
