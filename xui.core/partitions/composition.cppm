export module xui.core : composition;
// ^^^ [[xui.core]] standard object traits vvv

import std;

// vvv constant callable value vvv.
namespace xui {
	template<class T>
	struct callable {
		using type = T;

		T value;

		consteval callable(const T& Val)
			: value{ Val } {};
	}; //~ callable

	template<class T, class... Ts>
	constexpr static inline bool _invoke_if_nothrow{
		std::invocable<T, Ts...> ? std::is_nothrow_invocable_v<T, Ts...> : true
	};

	template<class T, class... Ts>
	auto invoke_if(T&& Callable, Ts&&... Args) noexcept(_invoke_if_nothrow<T, Ts...>) {
		if constexpr(std::invocable<T, Ts...>) {
			std::invoke(std::forward<T>(Callable), std::forward<Ts>(Args)...);
		};
	};

	// vvv acts similar to a vftable vvv.
	export template<callable... Callables>
		struct callbacks {
		constexpr static inline std::tuple callables{
			Callables...
		};
	}; //~ callbacks

	// vvv object traits vvv.
	export template<class... Ts>
		struct traits {
		std::tuple<Ts...> values;

		constexpr traits(const Ts&... Vals)
			: values{ Vals... } {};

		constexpr auto& operator*() noexcept {
			return values;
		};
	}; //~ traits
}; //~ xui::details

namespace xui {
	export template<class Callables, class Traits>
	struct composition;

	// object composition, callbacks, traits vvv.
	export template<callable... Callbacks, class... Traits>
	struct composition<callbacks<Callbacks...>, traits<Traits...>> {
		using callbacks_t = callbacks<Callbacks...>;
		using traits_t = traits<Traits...>;

		traits_t traits;

		constexpr composition() = default;
		constexpr composition(const Traits&... Vals) noexcept(std::is_nothrow_constructible_v<traits_t, Traits...>)
			: traits{ Vals... } {};

		template<class... Ts>
		constexpr static inline bool _nothrow_invocables_v{
			(std::is_nothrow_invocable_v<typename decltype(Callbacks)::type, traits_t&, Ts...> && ...)
		};

		// vvv forwards arguments to any invocable callable vvv.
		template<class... Ts>
		constexpr auto operator()(Ts&&... Args) noexcept(_nothrow_invocables_v<Ts...>) {
			std::apply([&](const auto&... Callbacks) {
				(std::invoke([&]<class T>(const T& Callable) {
					xui::invoke_if(Callable.value, traits, std::forward<Ts>(Args)...);
				}, Callbacks), ...);
			}, callbacks_t::callables);
		};
	}; //~ composition
}; //~ xui::details
