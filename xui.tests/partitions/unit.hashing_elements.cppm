export module xui.test_units : unit.hashing_elements;
// ^^^ [[xui.test_units]] unit test for object linkage and argument forwarding vvv

import "headers\\cmacros.hpp";

import std;
import xui.core;

namespace xui::test_units {
	namespace spec {
		template<class T>
		xui_inline constexpr auto as_bytes(const T& Val) noexcept {
			if constexpr(std::ranges::range<T>) {
				return std::as_bytes(std::span(Val));
			}
			else {
				return std::as_bytes(std::span(std::addressof(Val), 1));
			};
		};

		// fnv1a hasher contexts
		struct fnv1a_context {
			constexpr static std::uint64_t prime{ 0x00000100000001B3 };
			std::uint64_t hash{ 0xcbf29ce484222325 };

			template<class T>
			constexpr auto operator()(const T& Val) noexcept {
				[&](const std::ranges::range auto&& Range) {
					auto fnv1a_hash_fn = [&](const std::byte& K) noexcept {
							hash = prime * (hash ^ std::to_integer<std::uint64_t>(K));
						};
				
					std::ranges::for_each(std::cbegin(Range), std::cend(Range), fnv1a_hash_fn);
					}(spec::as_bytes(Val));
				};
			}; //~ fnv1a context

		using group_callbacks = xui::callbacks<
			// invoke for linkages.
			[]<class... Ts>(xui::linkage<Ts...>& Linkages, spec::fnv1a_context& Ctx) {
				// invokes `xui::linkage<...>::operator().`
				std::invoke(Linkages, Ctx);

				// print hash.
				std::cout << Ctx.hash << std::endl;
			},
			// add some salt.
			[](spec::fnv1a_context& Ctx) {
				std::random_device rd;
				std::mt19937_64 engine(rd());

				std::uniform_int_distribution<std::uint64_t>
					distribution(0u, std::numeric_limits<std::uint64_t>::max());

				std::uint64_t random_value = distribution(engine);
				std::invoke(Ctx, random_value);

				// print salted hash.
				std::cout << Ctx.hash << std::endl;
			}>;

		using group = xui::composition<spec::group_callbacks, xui::notraits>;

		using hashable_callbacks = xui::callbacks<
			// Traits.values is either of `std::vector<T>` or `std::string`.
			[](const auto& Traits, spec::fnv1a_context& Ctx) {
				auto& [Value] = Traits.values;

				// invokes `spec::fnv1a_context::operator()`.
				std::invoke(Ctx, Value);
			}>;

		using string_traits = xui::traits<std::string>;
		using string = xui::composition<spec::hashable_callbacks, spec::string_traits>;

		template<class T>
		using number_range_traits = xui::traits<std::vector<T>>;

		template<class T>  requires std::is_arithmetic_v<T>
		using numbers = xui::composition<spec::hashable_callbacks, spec::number_range_traits<T>>;
	}; //~ spec(ification)

	export void hashing_elements() {
		auto g0 = xui::link(spec::group{}, spec::string{"Hello"}, spec::string{"World!"},
				spec::numbers<float>{{5.80f, 1.8415f, -0.3f, 900.5f, -3.0f}});
		 
		spec::fnv1a_context ctx{};
		std::invoke(g0, ctx);
	};
}; //~ xui::test