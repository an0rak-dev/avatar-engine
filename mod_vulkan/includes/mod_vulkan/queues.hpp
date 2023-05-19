#ifndef AVATAR_MODVULKAN_QUEUES
#define AVATAR_MODVULKAN_QUEUES

#include <mod_vulkan/data.hpp>

bool queues_are_complete(struct queue_families& families);
void queues_reset(struct queue_families& families);
void queues_prepare_info(struct queue_families& families, VkDeviceQueueCreateInfo* create_infos, int* create_infos_size);

#endif
