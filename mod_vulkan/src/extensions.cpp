#include <mod_vulkan/extensions.hpp>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int available_extensions_count = 0;
VkExtensionProperties available[MAX_AVAILABLE_EXTENSIONS] = {};

int modvulkan_init_extensions(struct av_modvulkan_extensions& extensions) {
	if (available_extensions_count == 0) {
		if (VK_SUCCESS != vkEnumerateInstanceExtensionProperties(NULL, &available_extensions_count, NULL)) {
			return 1;
		}
		if (VK_SUCCESS != vkEnumerateInstanceExtensionProperties(NULL, &available_extensions_count, available)) {
			return 2;
		}
	}
	extensions.enabled_count = 0;
	for (int i = 0; i < MAX_ENABLED_EXTENSIONS; i++) {
		extensions.enabled[i] = NULL;
	}
	return 0;
}

void modvulkan_enable(struct av_modvulkan_extensions& extensions, const char* extension_name) {
	if (extensions.enabled_count == MAX_ENABLED_EXTENSIONS) {
		return;
	}
	extensions.enabled[extensions.enabled_count++] = extension_name;
}

bool modvulkan_validate_extensions(struct av_modvulkan_extensions& extensions) {
	for (int i = 0; i < extensions.enabled_count; i++) {
		bool found = false;
		for (unsigned int j = 0; j < available_extensions_count; j++) {
			if (0 == strncmp(available[j].extensionName, extensions.enabled[i], MAX_EXTENSION_LENGTH)) {
				found = true;
				break;
			}
		}
		if (!found) {
			printf("Extension %s isn't supported.\n", extensions.enabled[i]);
			return false;
		}
	}
	return true;
}