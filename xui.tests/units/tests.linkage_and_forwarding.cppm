export module xui.tests : linkage_and_forwarding;
// ^^^ [[xui.test]] unit test for object linkage and argument forwarding vvv

import xui.core;
import std;

namespace xui::tests {
	namespace en {
		// composition 0
		using traits_0 = xui::traits<std::string>;
		using callbacks_0 = xui::callbacks<
			// vvv prints object hello message vvv
			[]<class... Ts>(xui::traits<Ts...>& Obj, xui::input_ctx& Ctx) noexcept {
				auto& [Str] = *Obj;
				std::cout << Str << " says hello!" << std::endl;
				Str.append(" says bye!");
			},
			// vvv prints the amount of objects being invoked and invokes vvv
			[]<class... Ts>(xui::linkage<Ts...>& Linkage, xui::input_ctx& Ctx) noexcept {
				std::cout << "invoking for " << Linkage.objects.size() << " objects" << std::endl;
				std::invoke(Linkage, Ctx);
			},
			// vvv prints object bye message vvv
			[]<class... Ts>(xui::traits<Ts...>& Obj, xui::input_ctx& Ctx) noexcept {
				auto& [Str] = *Obj;
				std::cout << Str << std::endl;
			}>;

		using composition_0 = xui::composition<callbacks_0, traits_0>;
		//~ composition 0
	}; //~ unit

	export auto linkage_and_forwarding() {
		auto linkage_0 = xui::link(en::composition_0{"Object 3"}, 
			en::composition_0{"Object 4"}, en::composition_0{"Object 5"}, en::composition_0{"Object 6"});

		auto linkage_1 = xui::link(en::composition_0{"object 1"},
			en::composition_0{"Object 2"}, std::move(linkage_0));
	
		xui::input_ctx ctx{ .att{nullptr} };

		std::invoke(linkage_1, ctx);
	};
}; //~ xui::tests