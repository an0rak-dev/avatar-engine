#ifndef AVATAR_INTERFACES_WINDX12
#define AVATAR_INTERFACES_WINDX12

#include <avatar/core/scene.hpp>
#include <avatar/platforms/winrt.hpp>
#include <avatar/renderers/directx12.hpp>

typedef struct s_windx12_interface {
		winrt_platform platform;
		dx12_renderer  renderer;
} av_windx12_interface;

int  windx12_allocate(av_windx12_interface *itf);
void windx12_destroy(av_windx12_interface *itf);

int  windx12_init(av_windx12_interface &itf, const wchar_t *app_name);
void windx12_start(av_windx12_interface &itf);
bool windx12_is_running(av_windx12_interface &itf);
void windx12_poll_event(av_windx12_interface &itf);
void windx12_render(av_windx12_interface &itf, av_scene &scene);

#define av_interface            av_windx12_interface
#define av_interface_allocate   windx12_allocate
#define av_interface_destroy    windx12_destroy
#define av_interface_init       windx12_init
#define av_interface_start      windx12_start
#define av_interface_is_running windx12_is_running
#define av_interface_poll_event windx12_poll_event
#define av_interface_render     windx12_render

#endif
