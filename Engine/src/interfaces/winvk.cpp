#include <interfaces/winvk.hpp>
#include <stdlib.h>

int winvk_allocate(av_winvk_interface* itf) {
	itf->platform = {};
	itf->renderer = {};
	if (0 != winrt_allocate(&itf->platform)) {
		return 1;
	}
	if (0 != vulkan_allocate(&itf->renderer)) {
		return 2;
	}
	return 0;
}

void winvk_destroy(av_winvk_interface* itf) {
	if (NULL == itf) {
		return;
	}
	vulkan_destroy(&itf->renderer);
	winrt_destroy(&itf->platform);
}

int winvk_init(av_winvk_interface& itf, const wchar_t* app_name) {
	if (0 != winrt_initalize(itf.platform, app_name, 1280, 720)) {
		return 1;
	}

	if (0 != vulkan_initialize(itf.renderer, KHRVK_WIN32)) {
		return 2;
	}
	return 0;
}

void winvk_start(av_winvk_interface& itf) {
	winrt_show(itf.platform);
}

bool winvk_is_running(av_winvk_interface& itf) {
	return WINRT_QUIT != itf.platform.last_event;
}

void winvk_poll_event(av_winvk_interface& itf) {
	winrt_wait_next_event(itf.platform);
}

void winvk_render(av_winvk_interface& itf, av_scene& scene) {

}