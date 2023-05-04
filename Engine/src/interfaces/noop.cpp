#include <interfaces/noop.hpp>
#include <stdio.h>

int noop_allocate(noop_interface* interface) {
	return 0;
}

void noop_destroy(noop_interface* interface) {

}

int noop_init(noop_interface& interface, const wchar_t* app_name) {
	return 0;
}

void noop_start(noop_interface& interface) {
	int events_count = 2048;
	printf("Setting %d mock events in queue\n", events_count);
	interface.remaining_events = events_count;
}

bool noop_is_running(noop_interface& interface) {
	if (interface.remaining_events <= 0) {
		printf("No more mock events in queue\n");
	}
	return interface.remaining_events > 0;
}

void noop_poll_event(noop_interface& interface) {
	printf("Polling new event\n");
	--interface.remaining_events;
}
