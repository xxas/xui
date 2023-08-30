import xui.tests;
import xui.test_units;

int tests() {
	auto units = xui::tests::link_units<
		xui::test_units::fail_assert,
		xui::test_units::hashing_elements
	>();

	xui::tests::unit_context ctx{};
	std::invoke(units, ctx);

	return 0;
};

int main() {
    return tests();
}