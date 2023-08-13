export module xui.core : contexts;

import std;

namespace xui {
	export struct graphics_ctx {

	}; //~ graphics ctx

	export struct input_ctx {
		const void* att;

		template<class T>
		constexpr auto set(const T& Val) {
			if(att == nullptr) att = &Val;
		};
	}; //~ input ctx
}; //~ xui