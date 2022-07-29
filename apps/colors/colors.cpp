#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../syscall.h"

constexpr int H_MARGIN = 28, W_MARGIN = 8, Y_OFFSET = 24, X_OFFSET = 4;
constexpr int WIDTH = 330, HEIGHT = 60;
constexpr int WX = 10, WY = 10;

constexpr int ZEROX = 10, ZEROY = 10, DX = 80, DY = 20;

extern "C" int main(int argc, char** argv) {
  auto [hwnd, errOpen] = SyscallOpenWindow(
    WIDTH + W_MARGIN, HEIGHT + H_MARGIN, WX, WY, "colors");
  if (errOpen) {
    printf("SyscallOpenWindow failed: %s\n", strerror(errOpen));
    exit(errOpen);
  }
  for (int i = 0; i < 8; i++) {
    int x = X_OFFSET + ZEROX + DX * (i % 4);
    int y = Y_OFFSET + ZEROY + DY * (i / 4);
    int color = 0;
    if (i & 1) color |= 0xff;
    if (i & 2) color |= 0xff00;
    if (i & 4) color |= 0xff0000;
    char colorValue[32];
    snprintf(colorValue, sizeof(colorValue), "0x%06x", color);
    SyscallWinWriteString(hwnd | LAYER_NO_REDRAW, x, y, color, colorValue);
  }
  SyscallWinRedraw(hwnd);
  for (;;) {
    AppEvent event;
    auto [n, err] = SyscallReadEvent(&event, 1);
    if (err) {
      printf("SysCallReadEvent failed: %s\n", strerror(err));
      SyscallCloseWindow(hwnd);
      exit(err);
    }
    if (n > 0 && event.type == AppEvent::kQuit) {
      SyscallCloseWindow(hwnd);
      exit(0);
    }
  }
}
