#ifndef AVATAR_MODVULKAN_DATA
#define AVATAR_MODVULKAN_DATA

#include <vulkan/vulkan.h>

struct vulkan_specifics {
	VkInstance instance;
	VkDevice device;
	VkCommandPool command_pool;
	VkCommandBuffer command_buffer;
};


#endif
