export module xui.core : common.type_traits;
// ^^^ [[xui.core]] type meta-programming vvv

import std;

namespace xui {
	export template<class T, class... Ts>
	concept same_as = (std::same_as<T, Ts> || ...);

	template<class T, class R = std::tuple<>>
	struct remove_duplicate_tuple_elements;

	template<class... Ts>
	struct remove_duplicate_tuple_elements<std::tuple<>, std::tuple<Ts...>>
		: std::type_identity<std::tuple<Ts...>> {};

	template<class T, class... Ts, class... RTs>
	struct remove_duplicate_tuple_elements<std::tuple<T, Ts...>, std::tuple<RTs...>>
		: xui::remove_duplicate_tuple_elements<std::tuple<Ts...>,
		std::conditional_t<xui::same_as<T, RTs...>, std::tuple<RTs...>, std::tuple<RTs..., T>>> {};

	template<class... Ts>
	using remove_duplicate_tuple_elements_t = typename xui::remove_duplicate_tuple_elements<Ts...>::type;

	template<class T>
	struct tuple_to_variant;

	template<class... Ts>
	struct tuple_to_variant<std::tuple<Ts...>> : std::type_identity<std::variant<Ts...>> {};

	template<>
	struct tuple_to_variant<std::tuple<>> : std::type_identity<std::variant<void*>> {};

	template<class T>
	using tuple_to_variant_t = typename xui::tuple_to_variant<T>::type;

	export template<class... Ts>
	using variants = xui::tuple_to_variant_t<xui::remove_duplicate_tuple_elements_t<std::tuple<Ts...>>>;

	template<class T>
	struct is_smart_pointer: std::false_type {};

	template<class T>
	struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

	template<class T>
	struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

	template<class T>
	concept smart_pointer = is_smart_pointer<T>::value;

	export template<class T>
	concept pointer = std::is_pointer_v<T>;

	export template<class T>
	concept any_pointer = xui::pointer<T> || xui::smart_pointer<T>;
}; //~ xui