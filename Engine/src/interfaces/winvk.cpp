#include <interfaces/winvk.hpp>
#include <stdlib.h>

int winvk_allocate(winvk_interface* itf) {
	itf->platform = {};
	if (0 != winrt_allocate(&itf->platform)) {
		return 1;
	}
	return 0;
}

void winvk_destroy(winvk_interface* itf) {
	if (NULL == itf) {
		return;
	}
	winrt_destroy(&itf->platform);
}

int winvk_init(winvk_interface& itf, const wchar_t* app_name) {
	return winrt_initalize(itf.platform, app_name, 1280, 720);
}

void winvk_start(winvk_interface& itf) {
	winrt_show(itf.platform);
}

bool winvk_is_running(winvk_interface& itf) {
	return QUIT != itf.platform.last_event;
}

void winvk_poll_event(winvk_interface& itf) {
	winrt_wait_next_event(itf.platform);
}