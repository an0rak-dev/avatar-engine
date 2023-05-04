#ifndef AVATAR_INTERFACES_WINVK
#define AVATAR_INTERFACES_WINVK

#include <platforms/winrt.hpp>

struct winvk {
	struct winrt_data* window;
	enum winrt_event last_event;
};

int winvk_create(struct winvk* out_instance, const wchar_t* app_name, unsigned int width, unsigned int height);
void winvk_destroy(struct winvk& instance);
void winvk_start(struct winvk& instance);
bool winvk_was_quit_requested(struct winvk& instance);
void winvk_wait_next_event(struct winvk& instance);


#endif
