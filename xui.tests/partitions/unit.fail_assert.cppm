export module xui.test : unit.fail_assert;
// ^^^ [[xui.test]] unit test for invoking and constructing object compositions vvv

import "headers\\cmacros.hpp";

import std;
import xui.core;

namespace xui::test {
	export auto fail_assert() {
		xui_assert(false, "Test assert");
	};
}; //~ xui::test