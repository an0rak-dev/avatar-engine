#ifndef AVATAR_INTERFACE_NOOP
#define AVATAR_INTERFACE_NOOP


typedef struct noop_interface {
	int remaining_events;
} noop_interface;

int noop_allocate(noop_interface* interface);
void noop_destroy(noop_interface* interface);

int noop_init(noop_interface& interface, const wchar_t* app_name);
void noop_start(noop_interface& interface);
bool noop_is_running(noop_interface& interface);
void noop_poll_event(noop_interface& interface);

#define interface noop_interface
#define interface_allocate noop_allocate
#define interface_destroy noop_destroy
#define interface_init noop_init
#define interface_start noop_start
#define interface_is_running noop_is_running
#define interface_poll_event noop_poll_event

#endif
