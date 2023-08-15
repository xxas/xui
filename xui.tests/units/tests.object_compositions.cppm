export module xui.tests : object_compositions;
// ^^^ [[xui.test]] test object compositions vvv

import xui.core;
import std;

namespace xui::tests {
	namespace en {
		// composition 0
		using traits_0 = traits<float, float>;
		using callbacks_0 = callbacks<
			// vvv prints string trait vvv
			[]<class... Ts>(traits<Ts...>& Obj, input_ctx& Ctx) noexcept {
				auto& [Float32, Str] = Obj.values;
				std::cout << std::format("f32: {}f\nf32: {}f\n", Str, Float32);
			},
			[]<class... Ts>(traits<Ts...>& Obj, input_ctx& Ctx) noexcept {
				std::cout << std::format("sizeof: {}\n\n", sizeof(decltype(Obj)));
			},
			[]<class... Ts>(traits<Ts...>& Obj, graphics_ctx& Ctx) noexcept {
				std::cout << "graphics rendered\n" << std::endl;
			} >;

		using composition_0 = composition<callbacks_0, traits_0>;
		//~ composition 0
	}; //~ unit

	export auto object_compositions() {
		en::composition_0 test_object{0.532f, .1f};
		en::composition_0 test_object_2{5.2f, .9f};

		input_ctx ctx{ .att{nullptr} };

		std::invoke(test_object, ctx);
		std::invoke(test_object_2, ctx);
	};
}; //~ xui::tests