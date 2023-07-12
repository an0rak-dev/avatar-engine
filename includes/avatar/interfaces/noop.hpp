#ifndef AVATAR_INTERFACE_NOOP
#define AVATAR_INTERFACE_NOOP

#include <avatar/core/scene.hpp>

typedef struct s_noop_interface {
		int remaining_events;
} av_noop_interface;

int  noop_allocate(av_noop_interface *itf);
void noop_destroy(av_noop_interface *itf);

int  noop_init(av_noop_interface &itf, const wchar_t *app_name);
void noop_start(av_noop_interface &itf);
bool noop_is_running(av_noop_interface &itf);
void noop_poll_event(av_noop_interface &itf);
void noop_render(av_noop_interface &itf, av_scene &scene);

#define av_interface            noop_interface
#define av_interface_allocate   noop_allocate
#define av_interface_destroy    noop_destroy
#define av_interface_init       noop_init
#define av_interface_start      noop_start
#define av_interface_is_running noop_is_running
#define av_interface_poll_event noop_poll_event
#define av_interface_render     noop_render

#endif
