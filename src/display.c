#include "display.h"
#include "chip8.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <stdint.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture;
static uint32_t pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];

static void bitmap_to_RGBA(uint64_t display[DISPLAY_HEIGHT],
                           uint32_t (*pixels)[DISPLAY_WIDTH * DISPLAY_HEIGHT]) {
  uint64_t temp;
  uint64_t bitmap;
  for (int i = 0; i < DISPLAY_HEIGHT; i++) {
    bitmap = 0x8000000000000000; // start at binary 100000...
    for (int j = 0; j < DISPLAY_WIDTH; j++) {
      temp = !!(display[i] & bitmap);
      (*pixels)[i * DISPLAY_WIDTH + j] = (temp ? COLOR_HEX : 0x000000FF);
      bitmap = bitmap >> 1;
    }
  }
}

int init_display() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Chip 8", DISPLAY_WIDTH * 10,
                                   DISPLAY_HEIGHT * 10, SDL_WINDOW_RESIZABLE,
                                   &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH,
                              DISPLAY_HEIGHT);

  return SDL_APP_CONTINUE;
}

int render_frame(Chip8_state *chip8_state) {
  bitmap_to_RGBA(chip8_state->display, &pixels);

  if (!SDL_UpdateTexture(texture, NULL, pixels,
                         DISPLAY_WIDTH * sizeof(uint32_t))) {
    SDL_Log("Couldn't update texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderDrawColor(renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B,
                         SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  if (!SDL_RenderTexture(renderer, texture, NULL, NULL) ||
      !SDL_RenderPresent(renderer)) {
    SDL_Log("Couldn't render screen: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}
