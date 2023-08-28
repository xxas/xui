export module xui.test : framework;
// ^^^ [[xui.test]] test frameworking vvv

import "headers\\cmacros.hpp";

import std;
import xui.core;

import :unit.fail_assert;
import :unit.hashing_elements;

namespace xui::test {
	struct unit_context {
		size_t passes{}, fails{};

		template<class T>
		void operator()(T&& Callable) noexcept {
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
		[]<class... Ts>(xui::linkage<Ts...>& Linkages, test::unit_context& Ctx) noexcept {
			std::cout << "\n===================== xui test(s) ====================\n\n";
			std::invoke(Linkages, Ctx);
		},
		[](test::unit_context& Ctx) noexcept {
			std::cout <<"\n========== finished with " << Ctx.passes << " passed, " << Ctx.fails << " failed ==========\n";
		}>;

	using collective = xui::composition<test::collective_callbacks, xui::notraits>;

	template<xui::callable Callable>
	xui_inline void callable_wrapper(test::unit_context& Ctx) noexcept(std::is_nothrow_invocable_v<test::unit_context, typename decltype(Callable)::type>) {
		std::invoke(Ctx, Callable.value);
	};

	template<xui::callable Callable>
	using unit_callbacks = xui::callbacks<test::callable_wrapper<Callable>>;

	template<xui::callable Callable>
	using unit = xui::composition<test::unit_callbacks<Callable>, xui::notraits>;

	export template<xui::callable... Tests>
	xui_inline constexpr auto units() noexcept {
		return xui::link(test::collective{}, test::unit<Tests>{}...);
	};
;}; //~ xui::test