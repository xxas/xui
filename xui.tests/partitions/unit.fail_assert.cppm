export module xui.test_units : unit.fail_assert;
// ^^^ [[xui.test_units]] unit test for invoking and constructing object compositions vvv

import "headers\\cmacros.hpp";

import std;
import xui.core;

namespace xui::test_units {
	export auto fail_assert() {
		xui_assert(false, "Test assert");
	};
}; //~ xui::units