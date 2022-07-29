#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <vector>

extern "C" void main(int argc, char** argv) {
  if (argc != 3 && argc != 4) {
    printf("Usage: %s out_file write_size [type]\n",
      argc > 0 ? argv[0] : "fwrite");
    exit(1);
  }
  int type = argc == 3 ? 0 : atoi(argv[3]);
  FILE* fp = fopen(argv[1], "wb");
  if (fp == NULL) {
    printf("fopen failed: %s\n", strerror(errno));
    exit(1);
  }
  bool isError = false;
  long size = atol(argv[2]);
  std::vector<unsigned char> data(size, 'a');
  switch (type) {
    case 0:
      puts("writing 1 object whose size is \"size\"");
      if (fwrite(data.data(), size, 1, fp) != 1) {
        printf("fwrite failed\n");
        isError = true;
      }
      break;
    case 1:
      puts("writing \"size\" objects whose size is 1");
      if (fwrite(data.data(), 1, size, fp) != size) {
        printf("fwrite failed\n");
        isError = true;
      }
      break;
    default:
      printf("unknown type: %d\n", type);
      isError = true;
  }
  if (!fclose(fp)) {
    printf("fclose failed: %s\n", strerror(errno));
    isError = true;
  }
  exit(isError ? 1 : 0);
}
