#ifndef AVATAR_MODVULKAN_EXTENSIONS
#define AVATAR_MODVULKAN_EXTENSIONS

#include <vulkan/vulkan.h>

#define MAX_EXTENSION_LENGTH 64

#define MAX_ENABLED_EXTENSIONS 16
#define MAX_AVAILABLE_EXTENSIONS 256

int  modvulkan_extensions_init();
int modvulkan_extensions_enable_win32(const char** extensions, int* extensions_count);
bool modvulkan_extensions_validate(const char** extensions, int extensions_count);
bool modvulkan_extensions_validate_device(VkPhysicalDevice& device);
int modvulkan_extensions_enable_device(const char** extensions, unsigned int* extensions_count);

#endif
