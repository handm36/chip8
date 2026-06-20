#include "display.h"
#include "chip8.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int init_display() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Chip 8", DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  return SDL_APP_CONTINUE;
}

void render_frame(Chip8_state *chip8_state) {
  SDL_SetRenderDrawColor(renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B,
                         SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, COLOR_R, COLOR_G, COLOR_B, SDL_ALPHA_OPAQUE);
  // todo: actually render stuff
}
