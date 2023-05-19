#include <mod_vulkan/queues.hpp>

bool queues_are_complete(struct queue_families& families) {
	return families.graphic_index != UNDEFINED_QUEUE_FAMILY && families.present_index != UNDEFINED_QUEUE_FAMILY;
}

void queues_reset(struct queue_families& families) {
	families.graphic_index = UNDEFINED_QUEUE_FAMILY;
	families.present_index = UNDEFINED_QUEUE_FAMILY;
}

void queues_prepare_info(struct queue_families& families, VkDeviceQueueCreateInfo* create_infos, int* create_infos_size) {
	float queue_priority = 1.0f;
	if (*create_infos_size != MAX_QUEUES_PER_DEVICE) {
		*create_infos_size = 0;
		return;
	}

	int i = 0;
	//FIXME : Ugly as hell. Maybe using a set here huh ?
	if (families.graphic_index != UNDEFINED_QUEUE_FAMILY) {
		create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		create_infos[i].queueCount = 1;
		create_infos[i].pQueuePriorities = &queue_priority;
		create_infos[i].queueFamilyIndex = families.graphic_index;
		i++;
	}
	if (families.present_index != UNDEFINED_QUEUE_FAMILY && families.graphic_index != families.present_index) {
		create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		create_infos[i].queueCount = 1;
		create_infos[i].pQueuePriorities = &queue_priority;
		create_infos[i].queueFamilyIndex = families.present_index;
		i++;
	}
	*create_infos_size = i;
}