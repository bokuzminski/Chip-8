#include <SDL2/SDL.h>

typedef struct Display
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} Display;

void display_init(Display *display);
void buffer_graphics(Chip8 *ch8, uint32_t *buffer, SDL_Renderer *renderer);
void display_draw(Display *display, uint32_t *buffer);