#ifndef AVATAR_MODVULKAN_DATA
#define AVATAR_MODVULKAN_DATA

#include <vulkan/vulkan.h>

struct vulkan_specifics {
	VkInstance instance;
	VkDevice device;
	uint32_t queue_family_index;
	VkCommandPool command_pool;
	VkCommandBuffer command_buffer;
};

#define MODULE_VERSION 1
#define TARGETED_VULKAN_VERSION VK_API_VERSION_1_3

#endif
