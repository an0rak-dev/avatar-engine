#ifndef AVATAR_MODVULKAN_INSTANCE
#define AVATAR_MODVULKAN_INSTANCE

#include <mod_vulkan/data.hpp>

int modvulkan_instance_init(struct vulkan_specifics& specifics, enum av_supported_surface surface_kind);
void modvulkan_instance_destroy(struct vulkan_specifics& specifics);

#endif
