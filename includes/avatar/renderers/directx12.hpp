#ifndef AVATAR_RENDERERS_DIRECTX12
#define AVATAR_RENDERERS_DIRECTX12

#include <Windows.h>

struct dx12_specifics;

typedef struct dx12_renderer {
	struct dx12_specifics* specifics;
} dx12_renderer;

extern int dx12_allocate(dx12_renderer* renderer);
extern void dX12_destroy(dx12_renderer* renderer); 

extern int dx12_initialize(dx12_renderer& renderer, HWND attached_window);

extern void dx12_prepare(dx12_renderer& renderer);
extern void dx12_clear(dx12_renderer& renderer, float red, float green, float blue, float alpha);
extern int dx12_present(dx12_renderer& renderer);

#endif
