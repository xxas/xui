#ifndef _xui__cmacros_
// ^^^ [[xui.core]] common macros header vvv
	#define _xui__cmacros_

	#ifndef xui_inline
		#if defined(_MSC_VER)
			#define xui_inline __forceinline
		#elif defined(__GNUC__) || defined(__clang__)
			#define xui_inline inline __attribute__((always_inline))
		#else
			#define xui_inline inline
		#endif
	#endif  //~ xui_inline
#endif //~ _xui__cmacros_