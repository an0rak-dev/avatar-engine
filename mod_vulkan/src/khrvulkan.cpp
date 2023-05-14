#include <khrvulkan.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include <mod_vulkan/data.hpp>
#include <mod_vulkan/device.hpp>
#include <mod_vulkan/extensions.hpp> 
#include <mod_vulkan/instance.hpp>

int vulkan_allocate(av_vulkan* out_vulkan) {
	struct vulkan_specifics* allocated_specifics = (struct vulkan_specifics*)malloc(sizeof(struct vulkan_specifics));
	if (NULL == allocated_specifics) {
		return 1;
	}
	allocated_specifics->instance       = VK_NULL_HANDLE;
	allocated_specifics->device         = VK_NULL_HANDLE;
	allocated_specifics->command_pool   = VK_NULL_HANDLE;
	allocated_specifics->command_buffer = VK_NULL_HANDLE;
	out_vulkan->specifics = allocated_specifics;
	return 0;
}

void vulkan_destroy(av_vulkan* inout_vulkan) {
	if (NULL != inout_vulkan && NULL != inout_vulkan->specifics) {
		vkFreeCommandBuffers(inout_vulkan->specifics->device, inout_vulkan->specifics->command_pool, 1, &inout_vulkan->specifics->command_buffer);
		vkDestroyCommandPool(inout_vulkan->specifics->device, inout_vulkan->specifics->command_pool, NULL);
		modvulkan_device_destroy(*inout_vulkan->specifics);
		modvulkan_instance_destroy(*inout_vulkan->specifics);
		free(inout_vulkan->specifics);
		inout_vulkan->specifics = NULL;
	}
}

int vulkan_initialize(av_vulkan& vulkan, enum av_supported_surface surface_kind) {
	if (0 != modvulkan_instance_init(*vulkan.specifics, surface_kind)) {
		return 1;
	}

	struct device_requirements requirements = {};
	requirements.min_api_version = TARGETED_VULKAN_VERSION;
	requirements.device_queue_flags = VK_QUEUE_GRAPHICS_BIT;
	if (0 != modvulkan_device_init(*vulkan.specifics, requirements)) {
		return 2;
	}

	// Command Buffer
	VkCommandBufferAllocateInfo command_buffer_allocate_info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	VkCommandPoolCreateInfo command_pool_create_info = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	command_pool_create_info.queueFamilyIndex = vulkan.specifics->queue_family_index;
	if (VK_SUCCESS != vkCreateCommandPool(vulkan.specifics->device, &command_pool_create_info, NULL, &vulkan.specifics->command_pool)) {
		return 7;
	}
	command_buffer_allocate_info.commandPool = vulkan.specifics->command_pool;
	command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount = 1;
	if (VK_SUCCESS != vkAllocateCommandBuffers(vulkan.specifics->device, &command_buffer_allocate_info, &vulkan.specifics->command_buffer)) {
		return 8;
	}

	return 0;
}

int vulkan_reset(av_vulkan& vulkan) {
	VkCommandBufferBeginInfo begin_info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	if (VK_SUCCESS != vkBeginCommandBuffer(vulkan.specifics->command_buffer, &begin_info)) {
		return 1;
	}
	return 0;
}

void vulkan_clear(av_vulkan& vulkan, float red, float green, float blue, float alpha) {
	// TODO
}

void vulkan_present(av_vulkan& vulkan) {
	// TODO
}