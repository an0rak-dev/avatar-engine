#ifndef AVATAR_INTERFACE_NOOP
#define AVATAR_INTERFACE_NOOP

#include <core/scene.hpp>

typedef struct noop_interface {
	int remaining_events;
} av_noop_interface;

int noop_allocate(av_noop_interface* interface);
void noop_destroy(av_noop_interface* interface);

int noop_init(av_noop_interface& interface, const wchar_t* app_name);
void noop_start(av_noop_interface& interface);
bool noop_is_running(av_noop_interface& interface);
void noop_poll_event(av_noop_interface& interface);
void noop_render(av_noop_interface& interface, av_scene& scene);

#define interface av_noop_interface
#define interface_allocate noop_allocate
#define interface_destroy noop_destroy
#define interface_init noop_init
#define interface_start noop_start
#define interface_is_running noop_is_running
#define interface_poll_event noop_poll_event
#define interface_render noop_render;

#endif
