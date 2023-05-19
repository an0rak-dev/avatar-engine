#ifndef AVATAR_MODVULKAN_DATA
#define AVATAR_MODVULKAN_DATA

#include  <vulkan/vulkan.h>

struct queue_families {
	uint32_t graphic_index;
	uint32_t present_index;
};

struct vulkan_specifics {
	VkInstance instance;
	VkDevice device;
	struct queue_families queue_families;
	VkCommandPool command_pool;
	VkCommandBuffer command_buffer;
	VkSurfaceKHR surface;
};

#define UNDEFINED_QUEUE_FAMILY 999
#define MODULE_VERSION 1
#define TARGETED_VULKAN_VERSION VK_API_VERSION_1_3
#define MAX_QUEUES_PER_DEVICE 2

#endif
