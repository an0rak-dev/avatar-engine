#ifndef AVATAR_PLATFORMS_WINRT
#define AVATAR_PLATFORMS_WINRT

enum winrt_event {
	QUIT,
	NONE
};

struct winrt_data;

extern int winrt_allocate(struct winrt_data* out_window);
extern void winrt_destroy(struct winrt_data* window);

extern int winrt_init(struct winrt_data& out_window, const wchar_t* window_name, unsigned int width, unsigned int height);
extern void winrt_show(struct winrt_data& window);
extern enum winrt_event winrt_poll_event(struct winrt_data& window);

#endif
