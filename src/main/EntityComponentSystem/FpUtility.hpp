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
