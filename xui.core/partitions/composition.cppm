export module xui.core : composition;
// ^^^ [[xui.core]] object composition containing traits and callables vvv

import std;
import :functional;

namespace xui {
	export template<class T>
	struct callable {
		using type = T;

		T value;

		consteval callable(T Val)
			: value{Val} {};
	}; //~ callable

	export template<xui::callable... Callables>
	struct callbacks {
		constexpr static std::tuple callables{
			Callables...
		};
	}; //~ callbacks

	export template<class... Ts>
	struct traits {
		std::tuple<Ts...> values;

		constexpr traits(Ts&&... Vals) noexcept(std::is_nothrow_constructible_v<std::tuple<Ts...>, Ts...>)
			: values{Vals...} {};

		constexpr auto& operator*() noexcept {
			return values;
		};
	}; //~ traits

	export using notraits = xui::traits<>;
}; //~ xui

namespace xui {
	export template<class Callables, class Traits>
	struct composition;

	export template<xui::callable... Callbacks, class... Traits>
	struct composition<xui::callbacks<Callbacks...>, xui::traits<Traits...>> {
		using callbacks_t	= xui::callbacks<Callbacks...>;
		using traits_t		= xui::traits<Traits...>;

		traits_t traits;

		constexpr composition(xui::composition<callbacks_t, traits_t>&&) = default;
		constexpr composition(Traits&&... Vals) noexcept(std::is_nothrow_constructible_v<traits_t, Traits...>)
			: traits{std::forward<Traits>(Vals)...} {};

		template<class... Ts>
		constexpr static bool is_nothrow_invocable_v{
			(std::is_nothrow_invocable_v<typename decltype(Callbacks)::type, traits_t&, Ts...> && ...)
		};

		template<class... Ts>
		constexpr auto operator()(Ts&&... Args) noexcept(is_nothrow_invocable_v<Ts...>) {
			std::apply([&](const auto&... Callables) {
				(std::invoke([&]<class T>(const T& Callable) {
					xui::invoke_optionally(Callable.value, xui::forwards(traits),
						std::forward<Ts>(Args)...);
				}, Callables), ...);
			}, callbacks_t::callables);
		};
	}; //~ composition
}; //~ xui