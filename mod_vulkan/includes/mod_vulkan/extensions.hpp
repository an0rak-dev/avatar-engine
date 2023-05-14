#ifndef AVATAR_MODVULKAN_EXTENSIONS
#define AVATAR_MODVULKAN_EXTENSIONS

#define MAX_EXTENSION_LENGTH 64

#define MAX_ENABLED_EXTENSIONS 16
#define MAX_AVAILABLE_EXTENSIONS 256
#define KHR_WIN32_SURFACE_EXT "VK_KHR_win32_surface"

struct av_modvulkan_extensions {
	const char* enabled[MAX_ENABLED_EXTENSIONS];
	int enabled_count;
};

int  modvulkan_extensions_init(struct av_modvulkan_extensions& extensions);
void modvulkan_extensions_enable(struct av_modvulkan_extensions& extensions, const char* extension_name);
bool modvulkan_extensions_validate(struct av_modvulkan_extensions& extensions);

#endif
