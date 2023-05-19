#include <mod_vulkan/extensions.hpp>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
                           
#define KHR_WIN32_SURFACE_EXT "VK_KHR_win32_surface"
#define KHR_SURFACE_EXT "VK_KHR_surface"

unsigned int available_instances_extensions_count = 0;
VkExtensionProperties available_instances_extensions[MAX_AVAILABLE_EXTENSIONS] = {};

int modvulkan_extensions_init() {
	if (available_instances_extensions_count == 0) {
		if (VK_SUCCESS != vkEnumerateInstanceExtensionProperties(NULL, &available_instances_extensions_count, NULL)) {
			return 1;
		}
		if (VK_SUCCESS != vkEnumerateInstanceExtensionProperties(NULL, &available_instances_extensions_count, available_instances_extensions)) {
			return 2;
		}
	}
	return 0;
}

int modvulkan_extensions_enable_win32(const char** extensions, int* extensions_count) {
	if (NULL == extensions || NULL == extensions_count || 0 > *extensions_count || MAX_ENABLED_EXTENSIONS < *extensions_count) {
		return 1;
	}
	int idx = *extensions_count;
	extensions[idx++] = KHR_WIN32_SURFACE_EXT;
	extensions[idx++] = KHR_SURFACE_EXT;
	*extensions_count = idx;
	return 0;
}

bool modvulkan_extensions_validate(const char** extensions, int extensions_count) {
	for (int i = 0; i < extensions_count; i++) {
		bool found = false;
		for (unsigned int j = 0; j < available_instances_extensions_count; j++) {
			if (0 == strncmp(available_instances_extensions[j].extensionName, extensions[i], MAX_EXTENSION_LENGTH)) {
				found = true;
				break;
			}
		}
		if (!found) {
			printf("Extension %s isn't supported.\n", extensions[i]);
			return false;
		}
	}
	return true;
}