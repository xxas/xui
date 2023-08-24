export module xui.core : linkage;
// ^^^ [[xui.core]] composition object linkages and forwarding vvv

import std;
import :functional;
import :type_traits;
import :composition;

namespace xui {
	export template<class... Ts>
	struct linkage {
		constexpr static size_t size_bytes{std::max({ sizeof(Ts)... }) * sizeof...(Ts)},
			dynamic_size_bytes{(sizeof...(Ts) * sizeof(std::uintptr_t)) + (sizeof(Ts) + ...)};

		// chooses the smallest allocation size in bytes.
		constexpr static bool is_dynamically_allocated_v {
			size_bytes > dynamic_size_bytes
		};

		// conditional element type based on size of bytes being consumed.
		using element_t = std::conditional_t<is_dynamically_allocated_v,
				xui::variants<std::unique_ptr<Ts>...>, xui::variants<Ts...>>;

		using objects_t = std::vector<element_t>;

		objects_t objects;

		constexpr linkage(Ts&&... Objs) noexcept {
			if constexpr (is_dynamically_allocated_v) {
				(objects.push_back(std::make_unique<Ts>(std::move(Objs))), ...);
			} 
			else {
				(objects.emplace_back(std::move(Objs)), ...);
			};
		};

		template<class... T0s> 
		constexpr auto operator()(T0s&&... Args) {
			std::ranges::for_each(objects, [&](auto&& Element) {
				std::visit([&]<class T>(T& Visit) {
					if constexpr(std::invocable<T, T0s...>) {
						std::invoke(Visit, std::forward<T0s>(Args)...);
					}
					else if constexpr(xui::smart_pointer<T>) {
						if constexpr(std::invocable<typename T::element_type, T0s...>) {
							std::invoke(*Visit, std::forward<T0s>(Args)...);
						};
					};
				}, Element);
			});
		};
	}; //~ linkage

	template<class T, class... Ts>
	struct linker;

	export template<xui::callable... Callbacks, class... Traits, class... Objects>
	struct linker<xui::composition<xui::callbacks<Callbacks...>, xui::traits<Traits...>>, Objects...> {
		using traits_t		= xui::traits<Traits...>;
		using callbacks_t	= xui::callbacks<Callbacks...>;
		using composition_t	= xui::composition<callbacks_t, traits_t>;
		using linkage_t		= xui::linkage<Objects...>;

		composition_t object;
		linkage_t linkages;
			
		constexpr linker(xui::linker<composition_t, Objects...>&&) = default;
		constexpr linker(composition_t&& Obj, Objects&&... Objs) noexcept
			: object{std::move(Obj)}, linkages{std::move(Objs)...} {};

		template<class... Ts>
		constexpr static bool is_nothrow_invocable_v{
			(xui::is_nothrow_optional_invoke_v<typename decltype(Callbacks)::type,
				xui::arguments<traits_t&, linkage_t&>, Ts...> && ...)
		};

		template<class... Ts>
		constexpr auto operator()(Ts&&... Args) noexcept(is_nothrow_invocable_v<Ts...>) {
			std::apply([&](const auto&... Callables) {
				(std::invoke([&]<class T>(const T& Callable) {
					xui::invoke_optionally(Callable.value, xui::forwards(object.traits, linkages),
						std::forward<Ts>(Args)...);
				}, Callables), ...);
			}, callbacks_t::callables);
		};
	}; //~ linker
}; //~ xui

namespace xui {
	// constructs and returns `xui::linker` instance by consuming ownership of 
	// provided `Object` and any subsequent `Objects`.
	export template<class T, class... Ts>
	constexpr auto link(T&& Object, Ts&&... Objects) noexcept {
		return xui::linker<T, Ts...>{ std::move(Object), std::move(Objects)... };
	};
}; //~ xui