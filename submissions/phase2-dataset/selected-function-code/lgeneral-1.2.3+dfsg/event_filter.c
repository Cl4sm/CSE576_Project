int event_filter( const SDL_Event *event )
{
    switch ( event->type ) {
        case SDL_MOUSEMOTION:
            motion_x = event->motion.x;
            motion_y = event->motion.y;
            buttonstate[event->motion.state] = 1;
            motion = 1;
            return 0;
        case SDL_MOUSEBUTTONUP:
            buttonstate[event->button.button] = 0;
            buttonup = event->button.button;
            return 0;
        case SDL_MOUSEBUTTONDOWN:
            buttonstate[event->button.button] = 1;
            buttondown = event->button.button;
            return 0;
        case SDL_KEYUP:
            keystate[event->key.keysym.sym] = 0;
            return 0;
        case SDL_KEYDOWN:
            keystate[event->key.keysym.sym] = 1;
            return 1;
        case SDL_QUIT:
            sdl_quit = 1;
            return 0;
    }
    return 1;
}