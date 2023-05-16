#include <khrvulkan.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vulkan/vulkan.h>

#include <mod_vulkan/data.hpp>
#include <mod_vulkan/extensions.hpp> 

#define MAX_DEVICES_COUNT 16
#define MAX_QUEUE_FAMILIES 32
#define MODULE_VERSION 1

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
		vkDestroyDevice(inout_vulkan->specifics->device, NULL);
		vkDestroyInstance(inout_vulkan->specifics->instance, NULL);
		free(inout_vulkan->specifics);
		inout_vulkan->specifics = NULL;
	}
}

int vulkan_initialize(av_vulkan& vulkan, enum av_supported_surface surface_kind) {
	// Instance
	VkApplicationInfo application_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	VkInstanceCreateInfo create_info   = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	av_modvulkan_extensions extensions = {};
	int extensions_count               = 0;
	VkResult result                    = VK_SUCCESS;

	modvulkan_init_extensions(extensions);

	switch (surface_kind) {
	case KHRVK_WIN32:
		modvulkan_enable(extensions, KHR_WIN32_SURFACE_EXT);
		break;
	}
	if (!modvulkan_validate_extensions(extensions)) {
		return 1;
	}

	application_info.pApplicationName   = NULL; // Managed by the platform.
	application_info.applicationVersion = 0;    // Managed by the platform.
	application_info.pEngineName        = "Avatar";
	application_info.engineVersion      = MODULE_VERSION; // This should be linked only to the module, not all the engine.
	application_info.apiVersion         = VK_API_VERSION_1_3;

	create_info.pApplicationInfo        = &application_info;
	create_info.enabledLayerCount       = 0;
	create_info.ppEnabledLayerNames     = NULL;
	create_info.enabledExtensionCount   = extensions.enabled_count;
	create_info.ppEnabledExtensionNames = extensions.enabled;

	result = vkCreateInstance(&create_info, NULL, &vulkan.specifics->instance);
	if (VK_SUCCESS != result) {
		// TODO Error
		return 2;
	}

	// Device
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	unsigned int devices_count = 0;
	float queue_priority = 1.0f;
	VkPhysicalDevice devices[MAX_DEVICES_COUNT];
	VkDeviceCreateInfo device_create_info = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	VkDeviceQueueCreateInfo device_queue_create_info = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	VkPhysicalDeviceFeatures device_features = {};
	if (VK_SUCCESS != vkEnumeratePhysicalDevices(vulkan.specifics->instance, &devices_count, NULL)) {
		return 3;
	}
	if (VK_SUCCESS != vkEnumeratePhysicalDevices(vulkan.specifics->instance, &devices_count, devices)) {
		return 4;
	}
	for (int i = 0; i < devices_count; i++) {
		unsigned int queue_families_count = 0;
		VkPhysicalDeviceProperties properties = {};
		VkQueueFamilyProperties queue_families[MAX_QUEUE_FAMILIES] = {};
		vkGetPhysicalDeviceProperties(devices[i], &properties);
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_families_count, NULL);
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_families_count, queue_families);
		for (unsigned int j = 0; j < queue_families_count; j++) {
			if (properties.apiVersion >= application_info.apiVersion && queue_families[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				// TODO : Choose the device in a smarter way (the more VRAM, a high score of features...)
#ifdef _DEBUG
				printf("Using %s graphic card\n", properties.deviceName);
#endif
				physical_device = devices[i];
				device_queue_create_info.queueFamilyIndex = j;
				break;
			}
		}
	}
	if (VK_NULL_HANDLE == physical_device) {
		return 5;
	}
	device_queue_create_info.queueCount = 1;
	device_queue_create_info.pQueuePriorities = &queue_priority;
	
	device_create_info.pQueueCreateInfos = &device_queue_create_info;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pEnabledFeatures = &device_features;
	device_create_info.enabledExtensionCount = 0;
	if (VK_SUCCESS != vkCreateDevice(physical_device, &device_create_info, nullptr, &vulkan.specifics->device)) {
		return 6;
	}

	// Command Buffer
	VkCommandBufferAllocateInfo command_buffer_allocate_info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	VkCommandPoolCreateInfo command_pool_create_info = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	command_pool_create_info.queueFamilyIndex = device_queue_create_info.queueFamilyIndex;
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
