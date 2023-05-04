#include <stdio.h>
#include "platforms/winrt.hpp"

#include <Windows.h>

#define WINDOW_CLASS_NAME L"AvatarWindow"

struct winrt_data {
	HWND handle;
};

LRESULT window_procedure(HWND window_handle, UINT message, WPARAM wide_params, LPARAM long_params) {
	switch (message) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(window_handle, message, wide_params, long_params);
	}
}

static bool winrt_class_initiliazed = false;

int winrt_allocate(struct winrt_data* out_window) {
	out_window = (struct winrt_data*) malloc(sizeof(struct winrt_data));
	return out_window != NULL ? 0 : 1;
}

int winrt_init(struct winrt_data& window, const wchar_t* window_name, unsigned int width, unsigned int height) {
	HRESULT result = S_OK;
	HINSTANCE current_process = GetModuleHandle(NULL);
	if (!winrt_class_initiliazed) {
		WNDCLASSEX window_class = {
			sizeof(WNDCLASSEX),
			CS_HREDRAW | CS_VREDRAW, // Redraw all when the window moves along XY axis
			window_procedure,
			0, // No extra bytes for padding after this
			0, // No extra bytes for padding after window instances
			current_process,
			NULL, // Default icon
			NULL, // Default cursor
			NULL, // No default background
			NULL, // No default menu
			WINDOW_CLASS_NAME,
			NULL  // No small icon
		};

		if (0 == RegisterClassEx(&window_class)) {
			fprintf(stderr, "Unable to Register the window class : (code : %d)\n", GetLastError());
			return 1;
		}
		winrt_class_initiliazed = true;
	}

	window.handle = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		WINDOW_CLASS_NAME,
		window_name,
		WS_OVERLAPPEDWINDOW,          // Don't show the window at creation
		CW_USEDEFAULT, CW_USEDEFAULT, // Default X and Y position
		width, height,
		NULL,                         // No parent
		NULL,                         // No default menu
		current_process,
		NULL                          // No additional params
	);
	if (NULL == window.handle) {
		fprintf(stderr, "Unable to create the Window : (code : %d)\n", GetLastError());
		return 2;
	}
	return 0;
}

void winrt_destroy(struct winrt_data* window) {
	if (NULL == window) {
		return;
	}
	if (NULL != window->handle) {
		DestroyWindow(window->handle);
	}
	free(window);
	window = NULL;
}

void winrt_show(struct winrt_data& window) {
	ShowWindow(window.handle, SW_SHOW);
}

// FIXME : Close event not processed when using the close button as the 1st action. Need to do another action (like move the window) to use it.
enum winrt_event winrt_poll_event(struct winrt_data& window) {
	MSG msg = {};
	if (!GetMessage(&msg, window.handle, 0, 0)) {
		return QUIT;
	}
	TranslateMessage(&msg);
	DispatchMessageW(&msg);
	return NONE;
}