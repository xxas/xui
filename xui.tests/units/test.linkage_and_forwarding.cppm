export module xui.test : linkage_and_forwarding;
// ^^^ [[xui.test]] unit test for object linkage and argument forwarding vvv

import std;
import xui.core;

namespace xui::test {
	namespace spec {
		// converts `T` to `std::span<const std::uint8_t>`.
		template <class T>
		constexpr auto to_bytes(const T& Val) {
			const std::uint8_t* byte_ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(Val));
			return std::span<const std::uint8_t, sizeof(T)>(byte_ptr, sizeof(T));
		}

		// fnv1a hasher contexts
		struct fnv1a_context {
			constexpr static std::uint64_t prime{ 0x00000100000001B3 };
			std::uint64_t hash{ 0xcbf29ce484222325 };

			template<class T>
			constexpr auto operator()(const T& Val) {
				[&](const std::ranges::range auto&& Range) {
					auto fnv1a_hash_fn = [&](const std::convertible_to<std::uint64_t> auto& K) noexcept {
							hash = prime * (hash ^ static_cast<std::uint64_t>(K));
						};
				
					std::ranges::for_each(std::cbegin(Range), std::cend(Range), fnv1a_hash_fn);
					}(spec::to_bytes(Val));
				};
			}; //~ fnv1a context

		using group_callbacks = xui::callbacks<
			// invoke for linkages.
			[]<class... Ts>(xui::linkage<Ts...>& Linkages, spec::fnv1a_context& Ctx) {
				std::invoke(Linkages, Ctx);
			},
			// add some salt.
			[](spec::fnv1a_context& Ctx) {
				std::random_device rd;
				std::mt19937_64 engine(rd());

				std::uniform_int_distribution<std::uint64_t>
					distribution(0u, std::numeric_limits<std::uint64_t>::max());

				std::uint64_t random_value = distribution(engine);
				std::invoke(Ctx, random_value);
			}>;

		using group = xui::composition<spec::group_callbacks, xui::notraits>;

		using string_traits = xui::traits<std::string>;
		using string_callbacks = xui::callbacks<
			// hash std::string trait.
			[](string_traits& Traits, spec::fnv1a_context& Ctx) {
				auto& [String] = Traits;
				std::invoke(Ctx, Traits);
			}>;

		using string = xui::composition<spec::string_callbacks, spec::string_traits>;
	}; //~ spec(ification)

	export auto linkage_and_forwarding() {
		auto g0 = xui::link(spec::group{}, spec::string{"Hello"}, spec::string{"World!"});

		spec::fnv1a_context ctx{};
		std::invoke(g0, ctx);

		std::cout << ctx.hash << std::endl;
	};
}; //~ xui::test