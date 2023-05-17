#include <avatar/interfaces/winvk.hpp>
#include <stdio.h>
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
	vulkan_destroy_surface(itf->renderer);
	vulkan_destroy(&itf->renderer);
	winrt_destroy(&itf->platform);
}

int winvk_init(av_winvk_interface& itf, const wchar_t* app_name) {
#ifdef _DEBUG
	printf("Interface Configuration : Platform=Win32, Renderer=Vulkan\n");
#endif
	if (0 != winrt_initialize(itf.platform, app_name, 1280, 720)) {
		return 1;
	}

	if (0 != vulkan_initialize(itf.renderer, KHRVK_WIN32)) {
		return 2;
	}

	HINSTANCE app_instance = winrt_get_instance(itf.platform);
	HWND window_handle = winrt_get_handle(itf.platform);
	VkInstance instance = vulkan_get_instance(itf.renderer);

	struct VkWin32SurfaceCreateInfoKHR create_info = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
	create_info.hinstance = app_instance;
	create_info.hwnd = window_handle;
	VkSurfaceKHR win32_surface;
	VkResult result = vkCreateWin32SurfaceKHR(instance, &create_info, NULL, &win32_surface);
	if (VK_SUCCESS != result) {
		return 3;
	}
	if (VK_SUCCESS != vulkan_attach_surface(itf.renderer, win32_surface)) {
		return 4;
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
	vulkan_reset(itf.renderer);
	vulkan_clear(itf.renderer, scene.background_color.red, scene.background_color.green, scene.background_color.blue, scene.background_color.alpha);
	vulkan_present(itf.renderer);
}