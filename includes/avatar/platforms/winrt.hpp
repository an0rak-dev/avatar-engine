#ifndef AVATAR_PLATFORM_WINRT
#define AVATAR_PLATFORM_WINRT

#include <Windows.h>

struct winrt_specifics;

enum e_winrt_events {
	WINRT_QUIT,
	WINRT_NONE
};

typedef struct s_winrt_platform {
		enum e_winrt_events     last_event;
		struct winrt_specifics *specifics;
} winrt_platform;

extern int  winrt_allocate(winrt_platform *winrt);
extern void winrt_destroy(winrt_platform *winrt);

extern int  winrt_initialize(winrt_platform &winrt, const wchar_t *app_name, int width, int height);
extern void winrt_show(winrt_platform &winrt);
extern void winrt_wait_next_event(winrt_platform &winrt);

extern HWND winrt_get_window_handle(winrt_platform &winrt);

#endif
