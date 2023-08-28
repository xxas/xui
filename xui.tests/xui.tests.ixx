export module xui.test;
// ^^^ [[xui.test]] total collection of units vvv

import std;
import xui.core;

import :framework;
import :unit.fail_assert;
import :unit.hashing_elements;

namespace xui::test {
	export auto run() noexcept {
		auto units = test::units<test::fail_assert, test::hashing_elements>();

		test::unit_context ctx{};
		std::invoke(units, ctx);
	};
}; //~ xui::test