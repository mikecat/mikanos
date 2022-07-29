#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../syscall.h"

constexpr int H_MARGIN = 28, W_MARGIN = 8;
constexpr int WIDTH = 200, HEIGHT = 100;
constexpr int WX = 10, WY = 10;

extern "C" int main(int argc, char** argv) {
  auto [hwnd, errOpen] = SyscallOpenWindow(
    WIDTH + W_MARGIN, HEIGHT + H_MARGIN, WX, WY, "events");
  if (errOpen) {
    printf("SyscallOpenWindow failed: %s\n", strerror(errOpen));
    exit(errOpen);
  }
  for (;;) {
    AppEvent event;
    auto [n, err] = SyscallReadEvent(&event, 1);
    if (err) {
      printf("SysCallReadEvent failed: %s\n", strerror(err));
      SyscallCloseWindow(hwnd);
      exit(err);
    }
    if (n == 0) continue;
    auto& arg = event.arg;
    switch (event.type) {
      case AppEvent::kQuit:
        printf("kQuit\n");
        SyscallCloseWindow(hwnd);
        exit(0);
        break;
      case AppEvent::kMouseMove:
        printf("kMouseMove\n");
        printf("x = %d, y = %d, dx = %d, dy = %d, buttons = %d\n",
          arg.mouse_move.x, arg.mouse_move.y,
          arg.mouse_move.dx, arg.mouse_move.dy, arg.mouse_move.buttons);
        break;
      case AppEvent::kMouseButton:
        printf("kMouseButton\n");
        printf("x = %d, y = %d, press = %d, button = %d\n",
          arg.mouse_button.x, arg.mouse_button.y,
          arg.mouse_button.press, arg.mouse_button.button);
        break;
      case AppEvent::kTimerTimeout:
        printf("kTimerTimeout\n");
        printf("timeout = %lu, value = %d\n",
          arg.timer.timeout, arg.timer.value);
        break;
      case AppEvent::kKeyPush:
        printf("kKeyPush\n");
        printf("modifier = %d, keycode = %d, ascii = %d, press = %d\n",
          arg.keypush.modifier, arg.keypush.keycode,
          arg.keypush.ascii, arg.keypush.press);
        break;
      default:
        printf("unknown event %d\n", (int)event.type);
        break;
    }
  }
}
