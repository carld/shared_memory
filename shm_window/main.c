/* an event loop using lib SDL
 */

#include <SDL2/SDL.h>


#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

unsigned width = 640;
unsigned height = 480;
unsigned int frames = 0;
unsigned int start_tick;

#define NAME "pixels"

int main(int argc, char *argv[]) {
  SDL_Event ev;
  SDL_Window *window = 0;
  SDL_Renderer *renderer = 0;
  SDL_Texture *texture = 0;

  int fd = 0;
  void *pixels = 0;

/*
  printf("O_RDONLY %d \n", O_RDONLY);
  printf("O_RDWR   %d \n", O_RDWR);
  printf("S_IRUSR  %d \n", S_IRUSR);
  printf("S_IWUSR  %d \n", S_IWUSR);
  printf("PROT_READ %d \n", PROT_READ);
  printf("PROT_WRITE %d \n", PROT_WRITE);
  printf("MAP_SHARED %d \n", MAP_SHARED);
*/
  printf("creating buffer '%s'\n", NAME);
  fd = shm_open(NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    perror("shm_open");
    exit(-1);
  }
  if (ftruncate(fd, width * height * 4) < 0) {
    perror("ftruncate");
  }

  pixels = mmap(0, width * height * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (pixels == 0) {
    perror("mmap");
    exit(-1);
  }


  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
  }

  window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

  renderer = SDL_CreateRenderer(window, -1, 0);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_TARGET, width, height);

  SDL_ShowCursor(1);

  while(!SDL_PollEvent(&ev) || ev.type != SDL_QUIT) {

    if (ev.type == SDL_KEYDOWN) {
      if (ev.key.keysym.sym == SDLK_ESCAPE)
        break;
    }
    if (ev.type == SDL_QUIT)
      break;
    SDL_Delay(20);

    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof (Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  if (shm_unlink(NAME) < 0) {
    perror("shm_unlink");
  }
  munmap(pixels, width * height * 4);
  close(fd);

  return 0;
}

