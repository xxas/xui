export module xui.tests : linkage_and_forwarding;
// ^^^ [[xui.test]] test object linkage and selective forward invoking vvv

import xui.core;
import std;

namespace xui::tests {
	namespace en {
		// composition 0
		using traits_0 = traits<std::string>;
		using callbacks_0 = callbacks<
			// vvv prints object hello message vvv
			[]<class... Ts>(traits<Ts...>& Obj, input_ctx& Ctx) noexcept {
				auto& [Str] = *Obj;
				std::cout << Str << " says hello!" << std::endl;
				Str.append(" says bye!");
			},
			// vvv prints the amount of objects being invoked and invokes vvv
			[]<class... Ts>(linkage<Ts...>& Linkage, input_ctx& Ctx) noexcept {
				std::cout << "invoking for " << Linkage.objects.size() << " objects" << std::endl;
				std::invoke(Linkage, Ctx);
			},
			// vvv prints object bye message vvv
			[]<class... Ts>(traits<Ts...>& Obj, input_ctx& Ctx) noexcept {
				auto& [Str] = *Obj;
				std::cout << Str << std::endl;
			}>;

		using composition_0 = composition<callbacks_0, traits_0>;
		//~ composition 0
	}; //~ unit

	export auto linkage_and_forwarding() {
		auto linkage_0 = xui::link(en::composition_0{"Object 3"}, 
			en::composition_0{"Object 4"}, en::composition_0{"Object 5"}, en::composition_0{"Object 6"});

		auto linkage_1 = xui::link(en::composition_0{"object 1"},
			en::composition_0{"Object 2"}, std::move(linkage_0));
	
		input_ctx ctx{ .att{nullptr} };

		std::invoke(linkage_1, ctx);
	};
}; //~ xui::tests