export module xui.core : linkage;
// ^^^ [[xui.core]] standard invocable forwarding vvv

import std;
import :composition;
import :type_traits;

namespace xui {
	template<class T, class T0, class T1, class... Ts>
	constexpr static inline bool _invoke_if_else_nothrow{
		std::invocable<T, T0, Ts...> ? std::is_nothrow_invocable_v<T, Ts...> : 
		std::invocable<T, T1, Ts...> ? std::is_nothrow_invocable_v<T, Ts...> : true
	};

	template<class T, class T0, class T1, class... Ts>
	auto invoke_if_else(T&& Callable, T0&& F, T1&& S, Ts&&... Args) noexcept(_invoke_if_else_nothrow<T, T0, T1, Ts...>) {
		if constexpr (std::invocable<T, T0, Ts...>) {
			std::invoke(std::forward<T>(Callable), std::forward<T0>(F), std::forward<Ts>(Args)...);
		} else if constexpr (std::invocable<T, T1, Ts...>) {
			std::invoke(std::forward<T>(Callable), std::forward<T1>(S), std::forward<Ts>(Args)...);
		};
	};

	// Object dependency linkage
	export template<class... Ts>
	struct linkage {
		// unique_ptr so each element is aligned to the size of a pointer,
		// variant so we can store multiple different compositions.
		using variant = variants<Ts...>;

		using objects_t = std::vector<variant>;

		objects_t objects;

		template<class... Ts>
		constexpr auto operator()(Ts&&... Args) {
			std::ranges::for_each(objects, [&](auto& Object) {
				std::visit([&](auto& Visit) {
					if constexpr(std::invocable<decltype(Visit), Ts...>)
						std::invoke(Visit, std::forward<Ts>(Args)...);
				}, Object);
			});
		};
	}; //~ linkage

	template<class T, class... Ts>
	struct linker;

	export template<callable... Callbacks, class... Traits, class... Objects>
	struct linker<composition<callbacks<Callbacks...>, traits<Traits...>>, Objects...> {
		composition<callbacks<Callbacks...>, traits<Traits...>> object;
		linkage<Objects...> linkages;

		template<class... Ts>
		constexpr static inline bool _nothrow_invocables_v{
			((std::is_nothrow_invocable_v<typename decltype(Callbacks)::type, traits<Traits...>&, Ts...> ||
			  std::is_nothrow_invocable_v<typename decltype(Callbacks)::type, linkage<Objects...>&, Ts...>) && ...)
		};

		template<class... Ts>
		constexpr auto operator()(Ts&&... Args) noexcept(_nothrow_invocables_v<Ts...>) {
			std::apply([&](const auto&... Callbacks) {
				(std::invoke([&]<class T>(const T& Callable) {
					xui::invoke_if_else(Callable.value, object.traits, linkages, 
						std::forward<Ts>(Args)...);
				}, Callbacks), ...);
			}, callbacks<Callbacks...>::callables);
		};
	}; //~ linker
}; //~ details

namespace xui {
	export template<class T, class... Ts>
	constexpr auto link(const T& Object, const Ts&... Objects) noexcept {
		return linker<T, Ts...>{.object{ Object }, .linkages{.objects{ Objects... }}};
	};
}; //~ xui