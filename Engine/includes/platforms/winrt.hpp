#ifndef AVATAR_PLATFORM_WINRT
#define AVATAR_PLATFORM_WINRT

struct winrt_specifics;

enum winrt_events {
	WINRT_QUIT,
	WINRT_NONE
};

typedef struct wintr_platform {
	enum winrt_events last_event;
	struct winrt_specifics* specifics;
} av_winrt_platform;

extern int winrt_allocate(av_winrt_platform* winrt);
extern void winrt_destroy(av_winrt_platform* winrt);

extern int winrt_initalize(av_winrt_platform& winrt, const wchar_t* app_name, unsigned int width, unsigned int height);
extern void winrt_show(av_winrt_platform& winrt);
extern void winrt_wait_next_event(av_winrt_platform& winrt);

#endif
