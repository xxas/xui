export module xui.core : linkage;
// ^^^ [[xui.core]] standard invocable forwarding vvv

import std;
import :composition;
import :type_traits;

namespace xui {
	export template<class... Ts>
	struct linkage {
		using variants_t = variants<Ts...>;
		using objects_t = std::vector<variants_t>;

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
		using traits_t		= xui::traits<Traits...>;
		using callbacks_t	= xui::callbacks<Callbacks...>;
		using composition_t = xui::composition<callbacks_t, traits_t>;
		using linkage_t		= xui::linkage<Objects...>;

		composition_t object;
		linkage_t linkages;
			
		template<class... Ts>
		constexpr static inline bool is_nothrow_invocable_v{
			(xui::is_nothrow_optional_invoke_v<typename decltype(Callbacks)::type, xui::options<traits_t&, linkage_t&>, Ts...> && ...)
		};

		template<class... Ts>
		constexpr auto operator()(Ts&&... Args) noexcept(is_nothrow_invocable_v<Ts...>) {
			std::apply([&](const auto&... Callables) {
				(std::invoke([&]<class T>(const T& Callable) {
					xui::invoke_optionally(Callable.value, options<traits_t&, linkage_t&>{ object.traits, linkages },
						std::forward<Ts>(Args)...);
				}, Callables), ...);
			}, callbacks_t::callables);
		};
	}; //~ linker
}; //~ details

namespace xui {
	export template<class T, class... Ts>
	constexpr auto link(T&& Object, Ts&&... Objects) noexcept {
		return linker<T, Ts...>{ 
				.object{ std::move(Object) }, 
				.linkages{.objects{ std::move(Objects)...}}};
	};
}; //~ xui