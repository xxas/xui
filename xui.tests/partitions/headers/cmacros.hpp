#ifndef _xui__test__cmacro_
// ^^^ [[xui.test]] common macros header vvv
	#define _xui__test__cmacro_

	#ifndef xui_inline
		#if defined(_MSC_VER)
			#define xui_inline __forceinline
		#elif defined(__GNUC__) || defined(__clang__)
			#define xui_inline inline __attribute__((always_inline))
		#else
			#define xui_inline inline
		#endif
	#endif  //~ xui_inline

	#ifndef xui_assert
		#define xui_assert(Bool, Detail)\
			if (!(Bool)) {\
				const auto line = std::to_string(__LINE__);\
				const auto file = [FullPath = std::string(__FILE__)]() -> std::string {\
					return std::string(FullPath.substr(FullPath.find_last_of("\\/") + 1));\
				}();\
				const auto message = std::string("> Failed (")\
					.append(file)\
					.append(":L").append(line)\
					.append(")\n  ")\
					.append(Detail)\
					.append("\n");\
				throw std::runtime_error(message);\
			}
	#endif
#endif //~ _xui__test__cmacro_