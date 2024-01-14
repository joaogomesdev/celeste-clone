// #############################################
//              Platform globals
// #############################################

static bool running = true;

// #############################################
//              Platform Functions
// #############################################
bool platform_create_window(int width, int height, char *title);
void platform_update_window();

// #############################################
//              Windows Platform
// #############################################

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// #############################################
//               Windows Globals
// #############################################
static HWND window;

// #############################################
//          Windows Implementations
// #############################################
LRESULT CALLBACK windows_window_callback(HWND window, UINT msg, WPARAM wParam,
                                         LPARAM lParam) {
  LRESULT result = 0;

  switch (msg) {

  case WM_CLOSE: {
    running = false;
    break;
  }

  default: {
    result = DefWindowProcA(window, msg, wParam, lParam);
  }
  }

  return result;
}

bool platform_create_window(int width, int height, char *title) {

  HINSTANCE instance = GetModuleHandleA(0);

  WNDCLASSA ws = {};
  ws.hInstance = instance;
  ws.hIcon = LoadIcon(instance, IDI_APPLICATION);
  ws.hCursor = LoadCursor(NULL, IDC_ARROW);
  ws.lpszClassName = title; // This is NOT the title, just an unique identifier
  ws.lpfnWndProc =
      windows_window_callback; // Callback for input into the Window

  if (!RegisterClassA(&ws)) {
    return false;
  }

  int commonWindowStyles =
      WS_OVERLAPPEDWINDOW; // WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
                           // WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX

  window =
      CreateWindowExA(0,
                      title, // This references the className from the window
                      title, // The actual Title
                      commonWindowStyles, // Window Style
                      99, 100, width, height,
                      NULL, // parent
                      NULL, // menu
                      instance, NULL);

  if (window == NULL) {
    return false;
  }

  ShowWindow(window, SW_SHOW);

  return true;
}

void platform_update_window() {
  MSG msg;

  while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessageA(&msg); // calls the callback
  }
}

#endif

int main() {
  platform_create_window(1200, 720, "Celeste Motor");

  while (running) {
    // Update
    platform_update_window();
  }
  return 0;
}
