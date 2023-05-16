#ifndef AVATAR_INTERFACE_WINVK
#define AVATAR_INTERFACE_WINVK

#include <avatar/platforms/winrt.hpp>

#define VK_USE_PLATFORM_WIN32_KHR
#include <avatar/renderers/khrvulkan.hpp>

#include <avatar/core/scene.hpp>

typedef struct winvk_interface {
	av_winrt_platform platform;
	av_vulkan renderer;
} av_winvk_interface;

int winvk_allocate(av_winvk_interface* itf);
void winvk_destroy(av_winvk_interface* itf);

int winvk_init(av_winvk_interface& itf, const wchar_t* app_name);
void winvk_start(av_winvk_interface& itf);
bool winvk_is_running(av_winvk_interface& itf);
void winvk_poll_event(av_winvk_interface& itf);
void winvk_render(av_winvk_interface& itf, av_scene& scene);

#define av_interface av_winvk_interface
#define av_interface_allocate winvk_allocate
#define av_interface_destroy winvk_destroy
#define av_interface_init winvk_init
#define av_interface_start winvk_start
#define av_interface_is_running winvk_is_running
#define av_interface_poll_event winvk_poll_event
#define av_interface_render winvk_render

#endif
