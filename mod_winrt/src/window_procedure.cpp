#include <window_procedure.hpp>

static bool winrt_class_initiliazed = false;

LRESULT window_procedure(HWND window_handle, UINT message, WPARAM wide_params, LPARAM long_params) {
	switch (message) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(window_handle, message, wide_params, long_params);
	}
}

int register_window_class(HINSTANCE current_process) {
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
	return 0;
}
