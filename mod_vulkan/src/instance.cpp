#include <mod_vulkan/instance.hpp>

#include <avatar/renderers/khrvulkan.hpp>
#include <mod_vulkan/extensions.hpp>

int modvulkan_instance_init(struct vulkan_specifics& specifics, enum av_supported_surface surface_kind) {
	VkApplicationInfo application_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	av_modvulkan_extensions extensions = {};
	int extensions_count = 0;
	VkResult result = VK_SUCCESS;

	modvulkan_extensions_init(extensions);

	switch (surface_kind) {
	case KHRVK_WIN32:
		modvulkan_extensions_enable(extensions, KHR_WIN32_SURFACE_EXT);
		break;
	}
	if (!modvulkan_extensions_validate(extensions)) {
		return 1;
	}

	application_info.pApplicationName = NULL; // Managed by the platform.
	application_info.applicationVersion = 0;    // Managed by the platform.
	application_info.pEngineName = "Avatar";
	application_info.engineVersion = MODULE_VERSION; // This should be linked only to the module, not all the engine.
	application_info.apiVersion = TARGETED_VULKAN_VERSION;

	create_info.pApplicationInfo = &application_info;
	create_info.enabledLayerCount = 0;
	create_info.ppEnabledLayerNames = NULL;
	create_info.enabledExtensionCount = extensions.enabled_count;
	create_info.ppEnabledExtensionNames = extensions.enabled;

	result = vkCreateInstance(&create_info, NULL, &specifics.instance);
	if (VK_SUCCESS != result) {
		// TODO Error
		return 2;
	}

	return 0;
}

void modvulkan_instance_destroy(struct vulkan_specifics& specifics) {
	vkDestroyInstance(specifics.instance, NULL);
}