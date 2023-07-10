#include <avatar/interfaces/windx12.hpp>
#include <stdlib.h>

int windx12_allocate(av_windx12_interface *itf) {
	if (NULL == itf) {
		return 1;
	}

	if (0 != winrt_allocate(&itf->platform)) {
		return 2;
	}

	if (0 != dx12_allocate(&itf->renderer)) {
		return 3;
	}

	return 0;
}

void windx12_destroy(av_windx12_interface *itf) {
	if (NULL == itf) {
		return;
	}

	dX12_destroy(&itf->renderer);
	winrt_destroy(&itf->platform);
}

int windx12_init(av_windx12_interface &itf, const wchar_t *app_name) {
	if (0 != winrt_initalize(itf.platform, app_name, 1080, 720)) {
		return 1;
	}

	HWND handle = winrt_get_window_handle(itf.platform);
	if (0 != dx12_initialize(itf.renderer, handle)) {
		return 2;
	}

	return 0;
}

void windx12_start(av_windx12_interface &itf) {
	winrt_show(itf.platform);
}

bool windx12_is_running(av_windx12_interface &itf) {
	return WINRT_QUIT != itf.platform.last_event;
}

void windx12_poll_event(av_windx12_interface &itf) {
	winrt_wait_next_event(itf.platform);
}

void windx12_render(av_windx12_interface &itf, av_scene &scene) {
	dx12_prepare(itf.renderer);
	dx12_clear(
		itf.renderer,
		(float) scene.background_color.red,
		(float) scene.background_color.green,
		(float) scene.background_color.blue,
		(float) scene.background_color.alpha);
	dx12_present(itf.renderer);
}