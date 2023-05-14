#ifndef AVATAR_MODVULKAN_DEVICE
#define AVATAR_MODVULKAN_DEVICE

#include <mod_vulkan/data.hpp>

struct device_requirements {
	uint32_t min_api_version;
	VkQueueFlags device_queue_flags;
	int extensions_count;
	const char** device_extensions;
};

int modvulkan_device_init(struct vulkan_specifics& specifics, struct device_requirements& requirements);
void modvulkan_device_destroy(struct vulkan_specifics& specifics);

#endif
