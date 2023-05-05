#ifndef AVATAR_INTERFACE_WINVK
#define AVATAR_INTERFACE_WINVK

#include <platforms/winrt.hpp>

typedef struct winvk_interface {
	winrt_platform platform;
} winvk_interface;

int winvk_allocate(winvk_interface* itf);
void winvk_destroy(winvk_interface* itf);

int winvk_init(winvk_interface& itf, const wchar_t* app_name);
void winvk_start(winvk_interface& itf);
bool winvk_is_running(winvk_interface& itf);
void winvk_poll_event(winvk_interface& itf);

#define interface winvk_interface
#define interface_allocate winvk_allocate
#define interface_destroy winvk_destroy
#define interface_init winvk_init
#define interface_start winvk_start
#define interface_is_running winvk_is_running
#define interface_poll_event winvk_poll_event

#endif
