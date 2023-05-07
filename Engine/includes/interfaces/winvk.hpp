#ifndef AVATAR_INTERFACE_WINVK
#define AVATAR_INTERFACE_WINVK

#include <platforms/winrt.hpp>
#include <core/scene.hpp>

typedef struct winvk_interface {
	av_winrt_platform platform;
} av_winvk_interface;

int winvk_allocate(av_winvk_interface* itf);
void winvk_destroy(av_winvk_interface* itf);

int winvk_init(av_winvk_interface& itf, const wchar_t* app_name);
void winvk_start(av_winvk_interface& itf);
bool winvk_is_running(av_winvk_interface& itf);
void winvk_poll_event(av_winvk_interface& itf);
void winvk_render(av_winvk_interface& itf, av_scene& scene);

#define interface av_winvk_interface
#define interface_allocate winvk_allocate
#define interface_destroy winvk_destroy
#define interface_init winvk_init
#define interface_start winvk_start
#define interface_is_running winvk_is_running
#define interface_poll_event winvk_poll_event
#define interface_render winvk_render

#endif
