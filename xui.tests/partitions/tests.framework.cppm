export module xui.tests : tests.framework;
// ^^^ [[xui.tests]] test frameworking vvv

export import "headers\\cmacros.hpp";

import std;
import xui.core;

namespace xui::tests {
	export struct unit_context {
		size_t passes{}, fails{};

		template<class T>
		void operator()(T&& Callable) {
			try {
				std::invoke(std::forward<T>(Callable));
				++passes;
			}
			catch(const std::exception& Exc) {
				std::cout << Exc.what() << std::endl;
				++fails;
			};
		};
	}; //~ unit context

	using collective_callbacks = xui::callbacks<
		[]<class... Ts>(xui::linkage<Ts...>& Linkages, tests::unit_context& Ctx) {
			std::cout << "\n=============== ======= xui tests =====================\n\n";
			std::invoke(Linkages, Ctx);
		},
		[](tests::unit_context& Ctx) {
			std::cout <<"\n========== finished with " << Ctx.passes << " passed, " << Ctx.fails << " failed ==========\n";
		}>;

	using collective = xui::composition<tests::collective_callbacks, xui::notraits>;

	template<xui::callable Callable>
	xui_inline void callable_wrapper(tests::unit_context& Ctx) noexcept(std::is_nothrow_invocable_v<tests::unit_context, typename decltype(Callable)::type>) {
		std::invoke(Ctx, Callable.value);
	};

	template<xui::cgallable Callable>
	using unit_callbacks = xui::callbacks<tests::callable_wrapper<Callable>>;

	template<xui::callable Callable>
	using unit = xui::composition<tests::unit_callbacks<Callable>, xui::notraits>;

	// Makes a collection of unit tests.
	export template<xui::callable... Tests>
	xui_inline constexpr auto link_units() noexcept {
		return xui::link(tests::collective{}, tests::unit<Tests>{}...);
	};
;}; //~ xui::tests