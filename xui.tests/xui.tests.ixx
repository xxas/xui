export module xui.test;
// ^^^ [[xui.test]] total collection of units vvv

import xui.core;
import :object_compositions;
import :linkage_and_forwarding;

namespace xui::test {
	struct unit_context {
		size_t passes{}, fails{};

		template<class T>
		void operator()(T& Callable) {
			try {
				std::invoke(Callable);
				++passes;
			}
			catch(...) {
				++fails;
			};
		};
	}; //~ unit context

	using collective_callbacks = xui::callbacks<
		[]<class... Ts>(xui::linkage<Ts...>& Linkages, test::unit_context& Ctx) {
			std::invoke(Linkages, Ctx);
		},
		[](test::unit_context& Ctx) {
			std::cout << "passes: " << Ctx.passes << ", fails: " << Ctx.fails << std::endl;
		}>;

	using collective = xui::composition<test::collective_callbacks, xui::notraits>;

	template<xui::callable Callable>
	void callable_wrapper(test::unit_context& Ctx) {
		std::invoke(Ctx, Callable.value);
	};

	template<xui::callable Callable>
	using unit_callbacks = xui::callbacks<callable_wrapper<Callable>>;

	template<xui::callable Callable>
	using unit = xui::composition<test::unit_callbacks<Callable>, xui::notraits>;

	export auto run() noexcept {
		auto units = xui::link(test::collective{}, 
			test::unit<test::object_compositions>{},
			test::unit<test::linkage_and_forwarding>{});

		test::unit_context ctx{};
		std::invoke(units, ctx);
	};
;}; //~ xui::test