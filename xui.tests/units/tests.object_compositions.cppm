export module xui.tests : object_compositions;
// ^^^ [[xui.test]] unit test for invoking and constructing object compositions vvv

import xui.core;
import std;

namespace xui::tests {
	namespace en {
		// composition 0
		using traits_0 = xui::traits<float, float>;
		using callbacks_0 = xui::callbacks<
			// vvv prints string trait vvv
			[]<class... Ts>(xui::traits<Ts...>& Obj, xui::input_ctx& Ctx) noexcept {
				auto& [Float32, Str] = Obj.values;
				std::cout << std::format("f32: {}f\nf32: {}f\n", Str, Float32);
			},
			[]<class... Ts>(xui::traits<Ts...>& Obj, xui::input_ctx& Ctx) noexcept {
				std::cout << std::format("sizeof: {}\n\n", sizeof(decltype(Obj)));
			},
			[]<class... Ts>(xui::traits<Ts...>& Obj, xui::graphics_ctx& Ctx) noexcept {
				std::cout << "graphics rendered\n" << std::endl;
			} >;

		using composition_0 = composition<callbacks_0, traits_0>;
		//~ composition 0
	}; //~ unit

	export auto object_compositions() {
		en::composition_0 test_object{0.532f, .1f};
		en::composition_0 test_object_2{5.2f, .9f};

		xui::input_ctx ctx{ .att{nullptr} };

		std::invoke(test_object, ctx);
		std::invoke(test_object_2, ctx);
	};
}; //~ xui::tests