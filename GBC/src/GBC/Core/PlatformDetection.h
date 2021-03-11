#pragma once

#if _WIN32
	#if _WIN64
		#define GBC_PLATFORM_WINDOWS 1
	#else
		#error "x86 builds are not supported!"
	#endif
#else
	#error Unsupported platform!
#endif
