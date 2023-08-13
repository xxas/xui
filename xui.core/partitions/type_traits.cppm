export module xui.core : type_traits;

import std;

namespace xui {
	export template<class T, class... Ts>
	concept same_as = (std::same_as<T, Ts> || ...);

	template<class T, class R = std::tuple<>>
	struct remove_duplicate_element_types;

	template<class... Ts>
	struct remove_duplicate_element_types<std::tuple<>, std::tuple<Ts...>>
		: std::type_identity<std::tuple<Ts...>> {};

	template<class T, class... Ts, class... RTs>
	struct remove_duplicate_element_types<std::tuple<T, Ts...>, std::tuple<RTs...>>
		: remove_duplicate_element_types<std::tuple<Ts...>, 
		  std::conditional_t<same_as<T, RTs...>, std::tuple<RTs...>, std::tuple<RTs..., T>>> {};

	template<class T>
	struct convert_tuple_to_variant;

	template<class... Ts>
	struct convert_tuple_to_variant<std::tuple<Ts...>> {
		using type = std::variant<Ts...>;
	};

	template<>
	struct convert_tuple_to_variant<std::tuple<>> {
		using type = std::variant<void*>;
	};

	export template<class... Ts>
	using variants = typename convert_tuple_to_variant<
		typename remove_duplicate_element_types<std::tuple<Ts...>>::type
	>::type;
}; //~ xui::details