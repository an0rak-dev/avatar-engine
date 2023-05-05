#ifndef AVATAR_PLATFORM_WINRT
#define AVATAR_PLATFORM_WINRT

struct winrt_specifics;

enum winrt_events {
	QUIT,
	NONE
};

typedef struct wintr_platform {
	enum winrt_events last_event;
	struct winrt_specifics* specifics;
} winrt_platform;

extern int winrt_allocate(winrt_platform* winrt);
extern void winrt_destroy(winrt_platform* winrt);

extern int winrt_initalize(winrt_platform& winrt, const wchar_t* app_name, unsigned int width, unsigned int height);
extern void winrt_show(winrt_platform& winrt);
extern void winrt_wait_next_event(winrt_platform& winrt);

#endif
