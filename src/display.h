#include <SDL2/SDL.h>

typedef struct Display
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} Display;

void display_init(Display *display);
void display_draw(Display *display, uint32_t pixels[64 * 32]);
void display_cleanup(Display *display);