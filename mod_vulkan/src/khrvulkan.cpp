#include <avatar/renderers/khrvulkan.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include <mod_vulkan/data.hpp>
#include <mod_vulkan/extensions.hpp>
#include <mod_vulkan/queues.hpp>

#define MAX_DEVICES_COUNT 16
#define MAX_QUEUE_FAMILIES 32

int create_instance(struct vulkan_specifics* specifics, enum av_supported_surface surface_kind);
int create_device(struct vulkan_specifics* specifics);
int create_command_buffer(struct vulkan_specifics* specifics);
bool is_device_supported(VkPhysicalDevice& physical_device, VkSurfaceKHR& surface, struct queue_families& families);

int vulkan_allocate(av_vulkan* out_vulkan) {
	struct vulkan_specifics* allocated_specifics = (struct vulkan_specifics*)malloc(sizeof(struct vulkan_specifics));
	if (NULL == allocated_specifics) {
		return 1;
	}
	allocated_specifics->instance       = VK_NULL_HANDLE;
	allocated_specifics->device         = VK_NULL_HANDLE;
	allocated_specifics->command_pool   = VK_NULL_HANDLE;
	allocated_specifics->command_buffer = VK_NULL_HANDLE;
	queues_reset(allocated_specifics->queue_families);
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

void vulkan_destroy_surface(av_vulkan& inout_vulkan) {
	vkDestroySurfaceKHR(inout_vulkan.specifics->instance, inout_vulkan.specifics->surface, NULL);
}

int vulkan_initialize(av_vulkan& vulkan, enum av_supported_surface surface_kind) {
	if (0 != create_instance(vulkan.specifics, surface_kind)) {
		return 1;
	}
	return 0;
}

int vulkan_attach_surface(av_vulkan& vulkan, VkSurfaceKHR& surface) {
	vulkan.specifics->surface = surface;

	if (0 != create_device(vulkan.specifics)) {
		return 1;
	}

	if (0 != create_command_buffer(vulkan.specifics)) {
		return 2;
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

void vulkan_clear(av_vulkan& vulkan, double red, double green, double blue, double alpha) {
	// TODO
}

void vulkan_present(av_vulkan& vulkan) {
	// TODO
}

VkInstance vulkan_get_instance(av_vulkan& vulkan) {
	return vulkan.specifics->instance;
}

int create_instance(struct vulkan_specifics* specifics, enum av_supported_surface surface_kind) {
	VkApplicationInfo application_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	const char* instance_extensions[MAX_ENABLED_EXTENSIONS] = { NULL };
	int instance_extensions_count = 0;

	// Extensions
	modvulkan_extensions_init();
	switch (surface_kind) {
	case KHRVK_WIN32:
		modvulkan_extensions_enable_win32(instance_extensions, &instance_extensions_count);
		break;
	}
	if (!modvulkan_extensions_validate(instance_extensions, instance_extensions_count)) {
		return 1;
	}

	application_info.pApplicationName = NULL;   // Managed by the platform.
	application_info.applicationVersion = 0;    // Managed by the platform.
	application_info.pEngineName = "Avatar";
	application_info.engineVersion = MODULE_VERSION; // This should be linked only to the module, not all the engine.
	application_info.apiVersion = TARGETED_VULKAN_VERSION;

	create_info.pApplicationInfo = &application_info;
	create_info.enabledLayerCount = 0;
	create_info.ppEnabledLayerNames = NULL;
	create_info.enabledExtensionCount = instance_extensions_count;
	create_info.ppEnabledExtensionNames = instance_extensions;

	if (VK_SUCCESS != vkCreateInstance(&create_info, NULL, &specifics->instance)) {
		// TODO Error
		return 2;
	}

	return 0;
}

int create_device(struct vulkan_specifics* specifics) {
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	unsigned int devices_count = 0;
	float queue_priority = 1.0f;
	VkPhysicalDevice devices[MAX_DEVICES_COUNT];
	VkDeviceCreateInfo device_create_info = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	int total_queues_family = MAX_QUEUES_PER_DEVICE;
	VkDeviceQueueCreateInfo device_queues_create_info[MAX_QUEUES_PER_DEVICE] = {};
	VkPhysicalDeviceFeatures device_features = {};
	if (VK_SUCCESS != vkEnumeratePhysicalDevices(specifics->instance, &devices_count, NULL)) {
		return 3;
	}
	if (VK_SUCCESS != vkEnumeratePhysicalDevices(specifics->instance, &devices_count, devices)) {
		return 4;
	}
	for (unsigned int i = 0; i < devices_count; i++) {
		if (is_device_supported(devices[i], specifics->surface, specifics->queue_families)) {
			physical_device = devices[i];
			break;
		}
		// No match, we reset the queue_families indexes.
		queues_reset(specifics->queue_families);
	}
	if (VK_NULL_HANDLE == physical_device) {
		return 5;
	}

	queues_prepare_info(specifics->queue_families, device_queues_create_info, &total_queues_family);

	//const char* enabled_extensions = { VK_KHR_SURFACE_EXTENSION_NAME /*VK_KHR_SWAPCHAIN_EXTENSION_NAME */ };
	device_create_info.pQueueCreateInfos = device_queues_create_info;
	device_create_info.queueCreateInfoCount = total_queues_family;
	device_create_info.pEnabledFeatures = &device_features;
//	device_create_info.enabledExtensionCount = 1;
//	device_create_info.ppEnabledExtensionNames = &enabled_extensions;
	VkResult result = vkCreateDevice(physical_device, &device_create_info, nullptr, &specifics->device);
	if (VK_SUCCESS != result) {
		return 6;
	}
	return 0;
}

int create_command_buffer(struct vulkan_specifics* specifics) {
	VkCommandBufferAllocateInfo command_buffer_allocate_info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	VkCommandPoolCreateInfo command_pool_create_info = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	command_pool_create_info.queueFamilyIndex = specifics->queue_families.graphic_index;
	if (VK_SUCCESS != vkCreateCommandPool(specifics->device, &command_pool_create_info, NULL, &specifics->command_pool)) {
		return 1;
	}
	command_buffer_allocate_info.commandPool = specifics->command_pool;
	command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount = 1;
	if (VK_SUCCESS != vkAllocateCommandBuffers(specifics->device, &command_buffer_allocate_info, &specifics->command_buffer)) {
		return 2;
	}
	return 0;
}

bool is_device_supported(VkPhysicalDevice& physical_device, VkSurfaceKHR& surface, struct queue_families& families) {
	VkPhysicalDeviceProperties properties = {};
	vkGetPhysicalDeviceProperties(physical_device, &properties);
	// Condition 1 : the device should support the Targeted vulkan version
	if (properties.apiVersion < TARGETED_VULKAN_VERSION) {
		return false;
	}
	unsigned int queue_families_count = 0;
	VkQueueFamilyProperties queue_families[MAX_QUEUE_FAMILIES] = {};
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, NULL);
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, queue_families);
	for (unsigned int j = 0; j < queue_families_count; j++) {
		// Condition 2 : the device should have a queue family which supports the Graphic commands
		if (queue_families[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			families.graphic_index = j;
		}

		// Condition 3 : the device should have a queue family which supports the Present commands
		VkBool32 surface_supported = false;
		VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, j, surface, &surface_supported);
		if (surface_supported) {
			families.present_index = j;
		}

		if (queues_are_complete(families)) {
			break;
		}
	}

	if (queues_are_complete(families)) {
#ifdef _DEBUG
		printf("Using %s graphic card\n", properties.deviceName);
#endif
		return true;
	}

	return false;
}