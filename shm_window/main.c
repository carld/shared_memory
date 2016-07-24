/* an event loop using lib SDL
 */

#include <SDL2/SDL.h>

unsigned width = 640;
unsigned height = 480;
unsigned int pause = 0;
unsigned int frames = 0;
unsigned int start_tick;

int main(int argc, char *argv[]) {
  SDL_Event ev;
  SDL_Window *window = 0;

  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
  }

  window = SDL_CreateWindow("shm_window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

  SDL_ShowCursor(1);

  while(!SDL_PollEvent(&ev) || ev.type != SDL_QUIT) {

    if (ev.type == SDL_KEYDOWN) {
      if (ev.key.keysym.sym == SDLK_ESCAPE)
        break;
      if (ev.key.keysym.sym == SDLK_SPACE)
        pause = !pause;
    }
    if (ev.type == SDL_QUIT)
      break;
    SDL_Delay(20);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

