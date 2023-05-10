#include <khrvulkan.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vulkan/vulkan.h>

#include <mod_vulkan/extensions.hpp> 

#define MAX_DEVICES_COUNT 16
#define MODULE_VERSION 1

struct vulkan_specifics {
	VkInstance       instance;
	VkPhysicalDevice device;
};

int vulkan_allocate(av_vulkan* out_vulkan) {
	struct vulkan_specifics* allocated_specifics = (struct vulkan_specifics*)malloc(sizeof(struct vulkan_specifics));
	if (NULL == allocated_specifics) {
		return 1;
	}
	allocated_specifics->instance = VK_NULL_HANDLE;
	allocated_specifics->device = VK_NULL_HANDLE;
	out_vulkan->specifics = allocated_specifics;
	return 0;
}

void vulkan_destroy(av_vulkan* inout_vulkan) {
	if (NULL != inout_vulkan && NULL != inout_vulkan->specifics) {
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
	unsigned int devices_count = 0;
	VkPhysicalDevice devices[MAX_DEVICES_COUNT];
	if (VK_SUCCESS != vkEnumeratePhysicalDevices(vulkan.specifics->instance, &devices_count, NULL)) {
		return 3;
	}
	if (VK_SUCCESS != vkEnumeratePhysicalDevices(vulkan.specifics->instance, &devices_count, devices)) {
		return 4;
	}
	for (int i = 0; i < devices_count; i++) {
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(devices[i], &properties);
		if (properties.apiVersion >= application_info.apiVersion) {
			// TODO : Choose the device in a smarter way (the more VRAM, a high score of features...)
			printf("Using %s graphic card\n", properties.deviceName);
			vulkan.specifics->device = devices[i];
			break;
		}
	}
	if (VK_NULL_HANDLE == vulkan.specifics->device) {
		return 5;
	}

	return 0;
}
