# Core metaprogramming and static polymorphism

`xui.core` is a `C++23` module interface that aims to provide a wide range of versatile metaprogramming and static polymorphism techniques.\
Dual-licensed as Apache 2 / BSD-3.
## Quick start

`xui.core` requires a `C++23` compliant compiler.

1. Generate the `.ifc` files for `xui.core`.
>We've included a `MakeFile` for generating these with your native compiler.
2. Add the `.ifc` output directory as an `Additional BMI Directory`.
3. Use the new `import` declarative to `import xui.core`.
## Examples

`../partitions/object.composition.cppm`\
 specialization of composition:
```cpp
import xui.core;

using object_traits = xui::traits<std::string>;
using object_callbacks = xui::callbacks<
    [](object_traits& Traits, size_t& Index) {
        const auto&[string] = *Traits;    
        std::print("{1}:{0}", string, Index);
        ++Index;
    }>;

using object = xui::composition<object_callbacks, object_traits>;
```

`../partitions/object.linkage.cppm`\
 linking objects and forward Invocation:
```cpp
using object_callbacks = xui::callbacks<
  ...
    []<class... Ts>(xui::linkage<Ts...>& Linkages, size_t& Index) {
        std::invoke(Linkages, ++Index);
    }
  ...
    >;

void print() {
    auto group = xui::link(object{"Hello"}, object{"World!"});

    size_t index{};
    std::invoke(group, index);
};
```