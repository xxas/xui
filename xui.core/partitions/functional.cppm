export module xui.core : functional;
// ^^^ [[xui.core]] functional meta-programming vvv

import std;

namespace xui {
	export template<class... Ts>
	struct options {
		std::tuple<Ts...> arguments;

		constexpr options(Ts&&... Args)
			: arguments{ std::forward<Ts>(Args)... } {}
	}; //~ options

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
	}; //~ optional invoker

	export constexpr inline xui::optional_invoker invoke_optionally{};
}; //~ xui