export module xui.core : functional;
// ^^^ [[xui.core]] functional meta-programming vvv

import std;

namespace xui {
	// a group of arguments.
	export template<class... Ts>
	struct arguments {
		std::tuple<Ts...> args;

		constexpr arguments(Ts&&... Args)
			: args{ std::forward<Ts>(Args)... } {};
	}; //~ options

	template<class Callable, class T, class... Ts>
	struct is_nothrow_optional_invoke : std::true_type {};

	template<class Callable, class T, class... T0s, class... T1s>
	struct is_nothrow_optional_invoke<Callable, xui::arguments<T, T0s...>, T1s...> :
		std::conditional_t<std::invocable<Callable, T, T1s...>, std::is_nothrow_invocable<Callable, T, T1s...>, 
						 is_nothrow_optional_invoke<Callable, xui::arguments<T0s...>, T1s...>> {};

	// forwards a group of arguments.
	export template<class... Ts>
	constexpr xui::arguments<Ts&...> forwards(Ts&&... Vals) noexcept {
		return xui::arguments<Ts&...>{std::forward<Ts>(Vals)...};
	};
 
	export template<class Callable, class T, class... Ts>
	constexpr inline auto is_nothrow_optional_invoke_v = xui::is_nothrow_optional_invoke<Callable, T, Ts...>::value;


	// optionally invoke `callable` when it meets a combination of `Opt(ion)s` and Arg(ument)s.
	struct optional_invoker {
		template<class T, class... T0s>
		constexpr auto operator()(T&& Callable, xui::arguments<>&& Opts, T0s&&... Args) const noexcept {
			if constexpr (std::invocable<T, T0s...>) { // Provided with exclusively `Args...`.
				return std::invoke(std::forward<T>(Callable), std::forward<T0s>(Args)...);
			};
		};

		template<class T, class T0, class... T0s, class... T1s>
		constexpr auto operator()(T&& Callable, xui::arguments<T0, T0s...>&& Opts, T1s&&... Args) const noexcept(is_nothrow_optional_invoke_v<T, xui::arguments<T0, T0s...>, T1s...>) {
			if constexpr(std::invocable<T, T0, T1s...>) { // <<< combination of xui::arguments<...> followed by `Args...`.
				return std::invoke(std::forward<T>(Callable), std::forward<T0>(std::get<0>(Opts.args)),  std::forward<T1s>(Args)...);
			}
			else { // <<< recursively scan options and arguments.
				return std::invoke(*this, std::forward<T>(Callable), xui::arguments<T0s...>(std::forward<T0s>(std::get<T0s>(Opts.args))...),
					std::forward<T1s>(Args)...);
			};
		};
	}; //~ optional invoker

	export constexpr inline xui::optional_invoker invoke_optionally{};
}; //~ xui