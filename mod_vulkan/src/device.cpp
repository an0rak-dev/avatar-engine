#include <mod_vulkan/device.hpp>

#include <string.h>
#include <stdio.h>

#include <mod_vulkan/extensions.hpp>

#define MAX_DEVICES_COUNT 16
#define MAX_QUEUE_FAMILIES 32


int modvulkan_device_init(struct vulkan_specifics& specifics, struct device_requirements& requirements) {
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	unsigned int devices_count = 0;
	float queue_priority = 1.0f;
	VkPhysicalDevice devices[MAX_DEVICES_COUNT];
	VkDeviceCreateInfo device_create_info = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	VkDeviceQueueCreateInfo device_queue_create_info = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	VkPhysicalDeviceFeatures device_features = {};
	if (VK_SUCCESS != vkEnumeratePhysicalDevices(specifics.instance, &devices_count, NULL)) {
		return 3;
	}
	if (VK_SUCCESS != vkEnumeratePhysicalDevices(specifics.instance, &devices_count, devices)) {
		return 4;
	}
	for (int i = 0; i < devices_count; i++) {
		unsigned int queue_families_count = 0;
		unsigned int device_extension_count = 0;
		VkPhysicalDeviceProperties properties = {};
		VkQueueFamilyProperties queue_families[MAX_QUEUE_FAMILIES] = {};
		VkExtensionProperties device_extensions[MAX_AVAILABLE_EXTENSIONS] = {};
		vkGetPhysicalDeviceProperties(devices[i], &properties);
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_families_count, NULL);
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_families_count, queue_families);
		vkEnumerateDeviceExtensionProperties(devices[i], NULL, &device_extension_count, NULL);
		vkEnumerateDeviceExtensionProperties(devices[i], NULL, &device_extension_count, device_extensions);
		// TODO : Choose the device in a smarter way (the more VRAM, a high score of features...)
		if (properties.apiVersion < requirements.min_api_version) {
			continue; // Device doesn't support the minimum wanted apiVersion configured in application.
		}
		// TODO: Three for loops imbricated. Ugly as hell. Refactor this.
		for (unsigned int j = 0; j < queue_families_count; j++) {
			if (queue_families[j].queueFlags & requirements.device_queue_flags) {
				for (unsigned int k = 0; k < device_extension_count; k++) {
					if (0 == strncmp(device_extensions[k].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME, MAX_EXTENSION_LENGTH)) {
#ifdef _DEBUG
						printf("Using %s graphic card\n", properties.deviceName);
#endif
						physical_device = devices[i];
						specifics.queue_family_index = j;
						break;
					}
				}
			}
			if (VK_NULL_HANDLE != physical_device) {
				break;
			}
		}
	}
	if (VK_NULL_HANDLE == physical_device) {
		return 5;
	}
	device_queue_create_info.queueCount = 1;
	device_queue_create_info.pQueuePriorities = &queue_priority;
	device_queue_create_info.queueFamilyIndex = specifics.queue_family_index;

	const char* enabled_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	device_create_info.pQueueCreateInfos = &device_queue_create_info;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pEnabledFeatures = &device_features;
	device_create_info.enabledExtensionCount = 1;
	device_create_info.ppEnabledExtensionNames = &enabled_extensions;
	if (VK_SUCCESS != vkCreateDevice(physical_device, &device_create_info, nullptr, &specifics.device)) {
		return 6;
	}
	return 0;
}

void modvulkan_device_destroy(struct vulkan_specifics& specifics) {
	vkDestroyDevice(specifics.device, NULL);
}