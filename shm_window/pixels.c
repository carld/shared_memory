#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME "pixels"

int width = 640, height = 480;

int main(int argc, char *argv[]) {
  int fd;
  unsigned int *pixels;
  int x, y;
  unsigned 
      r = 0xff << 24,
      g = 0xff << 16,
      b = 0xff << 8,
      a = 0xff;
  if (argc >= 4) {
    r = atoi(argv[1]) & 0xff;
    g = atoi(argv[2]) & 0xff;
    b = atoi(argv[3]) & 0xff;
  }

  printf ("%x %x %x %x\n", r, g, b, a);
  r <<= 24;
  g <<= 16;
  b <<= 8;

  fd = shm_open(NAME, O_RDWR, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    perror("shm_open");
    exit(-1);
  }
  /*
  if (ftruncate(fd, width * height * 4) < 0) {
    perror("ftruncate");
  }
  */

  pixels = mmap(0, width * height * 4, PROT_WRITE, MAP_SHARED, fd, 0);
  if (pixels == 0) {
    perror("mmap");
    exit(-1);
  }

  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      pixels[y * width + x] = r | g | b | a;
    }
  }
/*
  if (shm_unlink(NAME) < 0) {
    perror("shm_unlink");
  }
  */
  munmap(pixels, width * height * 4);
  close(fd);
  return 0;
}
