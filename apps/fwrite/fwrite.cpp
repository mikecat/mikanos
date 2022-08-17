#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <vector>

int main(int argc, char** argv) {
  if (argc != 3 && argc != 4) {
    printf("Usage: %s out_file write_size [type]\n",
      argc > 0 ? argv[0] : "fwrite");
    return 1;
  }
  int type = argc == 3 ? 0 : atoi(argv[3]);
  FILE* fp = fopen(argv[1], "wb");
  if (fp == NULL) {
    printf("fopen failed: %s\n", strerror(errno));
    return 1;
  }
  bool isError = false;
  long size = atol(argv[2]);
  std::vector<unsigned char> data(size, 'a');
  switch (type) {
    case 0:
      printf("writing 1 object whose size is %ld\n", size);
      if (fwrite(data.data(), size, 1, fp) != 1) {
        printf("fwrite failed\n");
        isError = true;
      }
      break;
    case 1:
      printf("writing %ld objects whose size is 1\n", size);
      if (fwrite(data.data(), 1, size, fp) != size) {
        printf("fwrite failed\n");
        isError = true;
      }
      break;
    default:
      printf("unknown type: %d\n", type);
      isError = true;
  }
  if (fclose(fp) == EOF) {
    printf("fclose failed: %s\n", strerror(errno));
    isError = true;
  }
  return isError ? 1 : 0;
}
