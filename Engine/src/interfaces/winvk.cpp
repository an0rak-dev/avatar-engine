#include <interfaces/winvk.hpp>
#include <stdio.h>

int winvk_allocate(winvk_interface* interface) {
	return 0;
}

void winvk_destroy(winvk_interface* interface) {

}

int winvk_init(winvk_interface& interface, const wchar_t* app_name) {
	return 0;
}

void winvk_start(winvk_interface& interface) {
	
}

bool winvk_is_running(winvk_interface& interface) {
	return false;
}

void winvk_poll_event(winvk_interface& interface) {
	
}