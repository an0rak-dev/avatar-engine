#ifndef AVATAR_RENDERERS_KHRVULKAN
#define AVATAR_RENDERERS_KHRVULKAN

struct vulkan_specifics;

enum av_supported_surface {
	KHRVK_WIN32,
	KHRVK_NONE = 999
};

typedef struct vulkan {
	struct vulkan_specifics* specifics;
} av_vulkan;

extern int vulkan_allocate(av_vulkan* out_vulkan);
extern void vulkan_destroy(av_vulkan* inout_vulkan);

extern int vulkan_initialize(av_vulkan& vulkan, enum av_supported_surface surface_kind);

extern int vulkan_reset(av_vulkan& vulkan);
extern void vulkan_clear(av_vulkan& vulkan, float red, float green, float blue, float alpha);
extern void vulkan_present(av_vulkan& vulkan);

#endif
