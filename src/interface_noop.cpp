#include <avatar/interfaces/noop.hpp>
#include <stdio.h>

int noop_allocate(av_noop_interface * /*itf*/) {
	return 0;
}

void noop_destroy(av_noop_interface * /*itf*/) {}

int noop_init(av_noop_interface & /*itf*/, const wchar_t * /*app_name*/) {
	return 0;
}

void noop_start(av_noop_interface &itf) {
	const int events_count = 2048;
	printf("Setting %d mock events in queue\n", events_count);
	itf.remaining_events = events_count;
}

bool noop_is_running(av_noop_interface &itf) {
	if (itf.remaining_events <= 0) {
		printf("No more mock events in queue\n");
	}
	return itf.remaining_events > 0;
}

void noop_poll_event(av_noop_interface &itf) {
	printf("Polling new event\n");
	--itf.remaining_events;
}

void noop_render(av_noop_interface & /*itf*/, av_scene & /*scene*/) {
	printf("Rendering scene\n");
}