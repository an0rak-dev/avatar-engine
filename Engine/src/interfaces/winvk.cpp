#include <interfaces/winvk.hpp>

#include <stdlib.h>

int winvk_create(struct winvk* out_instance, const wchar_t* app_name, unsigned int width, unsigned int height) {
	// TODO Use defines for error codes
	if (NULL == out_instance) {
		return 1;
	}
	if (0 != winrt_allocate(out_instance->window)) {
		return 2;
	}
	if (0 != winrt_init(*out_instance->window, app_name, width, height)) {
		return 3;
	}
	out_instance->last_event = NONE;
	return 0;
}

void winvk_destroy(struct winvk& instance) {
	winrt_destroy(instance.window);
}

void winvk_start(struct winvk& instance) {
	winrt_show(*instance.window);
}

bool winvk_was_quit_requested(struct winvk& instance) {
	return instance.last_event == QUIT;
}

void winvk_wait_next_event(struct winvk& instance) {
	enum winrt_event event = winrt_poll_event(*instance.window);
	instance.last_event = event;
}