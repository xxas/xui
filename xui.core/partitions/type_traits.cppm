export module xui.core : type_traits;
// ^^^ [[xui.core]] type meta-programming vvv

import std;

namespace xui {
	export template<class T, class... Ts>
	concept same_as = (std::same_as<T, Ts> || ...);

	template<class T, class R = std::tuple<>>
	struct remove_duplicate_tuple_elements;

	template<class... Ts>
	struct remove_duplicate_tuple_elements<std::tuple<>, std::tuple<Ts...>>
		: std::type_identity<std::tuple<Ts...>> 
	{};  //~ remove duplicate tuple elements

	template<class T, class... Ts, class... RTs>
	struct remove_duplicate_tuple_elements<std::tuple<T, Ts...>, std::tuple<RTs...>> : xui::remove_duplicate_tuple_elements<std::tuple<Ts...>,
						std::conditional_t<same_as<T, RTs...>, std::tuple<RTs...>, std::tuple<RTs..., T>>> 
	{}; //~ remove duplicate tuple elements

	template<class... Ts>
	using remove_duplicate_tuple_elements_t = xui::remove_duplicate_tuple_elements<Ts...>::type;

	template<class T>
	struct tuple_to_variant;

	template<class... Ts>
	struct tuple_to_variant<std::tuple<Ts...>> {
		using type = std::variant<Ts...>;
	}; //~ tuple to variant

	template<>
	struct tuple_to_variant<std::tuple<>> {
		using type = std::variant<void*>;
	}; //~ tuple to variant

	template<class T>
	using tuple_to_variant_t = xui::tuple_to_variant<T>::type;

	export template<class... Ts>
	using variants = xui::tuple_to_variant_t<xui::remove_duplicate_tuple_elements_t<std::tuple<Ts...>>>;
}; //~ xui