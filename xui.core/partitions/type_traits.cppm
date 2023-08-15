export module xui.core : type_traits;
// ^^^ [[xui.core]] standard type traits meta-programming vvv

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

	// removes any duplicate template arguments from `std::variant<Ts...>`.
	export template<class... Ts>
	using variants = xui::tuple_to_variant_t<xui::remove_duplicate_tuple_elements_t<std::tuple<Ts...>>>;
}; //~ xui::details

namespace xui {
	export template<class... Ts>
	struct options {
		std::tuple<Ts...> arguments;

		constexpr options(Ts&&... Args)
			: arguments{ std::forward<Ts>(Args)... } {}
	};

	template<class Callable, class T, class... Ts>
	struct is_nothrow_optional_invoke : std::true_type{};

	template<class Callable, class T, class... T0s, class... T1s>
	struct is_nothrow_optional_invoke<Callable, xui::options<T, T0s...>, T1s...> :
		std::conditional_t<std::invocable<Callable, T, T1s...>, std::is_nothrow_invocable<Callable, T, T1s...>, 
						 is_nothrow_optional_invoke<Callable, xui::options<T0s...>, T1s...>>
	{}; //~ is nothrow optional invoke

	export template<class Callable, class T, class... Ts>
	constexpr inline auto is_nothrow_optional_invoke_v = xui::is_nothrow_optional_invoke<Callable, T, Ts...>::value;

	struct optional_invoker {
		template<class T, class... T0s>
		constexpr auto operator()(T&& Callable, xui::options<>&& Opts, T0s&&... Args) const noexcept {};

		template<class T, class T0, class... T0s, class... T1s>
		constexpr auto operator()(T&& Callable, xui::options<T0, T0s...>&& Opts, T1s&&... Args) const noexcept(is_nothrow_optional_invoke_v<T, xui::options<T0, T0s...>, T1s...>) {
			if constexpr (std::invocable<T, T0, T1s...>) {
				return std::invoke(std::forward<T>(Callable), std::forward<T0>(std::get<0>(Opts.arguments)),  std::forward<T1s>(Args)...);
			}
			else {
				return std::invoke(*this, std::forward<T>(Callable), xui::options<T0s...>(std::forward<T0s>(std::get<T0s>(Opts.arguments))...),
					std::forward<T1s>(Args)...);
			};
		};
	}; //~ optional_invoker

	export constexpr inline optional_invoker invoke_optionally{};
}; //~ xui