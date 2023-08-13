export module xui.tests : object_compositions;
// ^^^ [[xui.test]] test object compositions vvv

import xui.core;
import std;

namespace xui::tests {
	namespace en {
		using traits = traits<float, std::string>;

		using callbacks = callbacks<
			// vvv prints traits vvv
			[](auto& Obj, input_ctx& Ctx) {
				auto& [Float32, Str] = Obj.values;
				std::cout << std::format("string: {}\nf32: {}f\n", Str, Float32);
			},
			// vvv prints sizeof traits vvv
			[](auto& Obj, input_ctx& Ctx) {
				std::cout << std::format("size of: {}\n\n", sizeof(traits));
			},
			// vvv doesn't get invoked vvv
			[](auto& Obj, graphics_ctx& Ctx) {
				std::cout << "graphics rendered" << std::endl;
			}>;

		using composition = composition<callbacks, traits>;
	}; //~ unit

	export auto object_compositions() {
		en::composition test_object{0.532f, "Object 1"};
		en::composition test_object_2{5.2f, "Object 2"};

		input_ctx ctx{ .att{nullptr} };

		std::invoke(test_object, ctx);
		std::invoke(test_object_2, ctx);
	};
}; //~ xui::tests