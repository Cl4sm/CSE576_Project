void JoystickMove()
{
        static Uint32 last_tick=0;
        Uint32 tick;
        Sint16 x,y;
        int MouseX, MouseY;

        if(!joy) return;

        x = SDL_JoystickGetAxis(joy, 0);
        y = SDL_JoystickGetAxis(joy, 1);

        SDL_GetMouseState(&MouseX, &MouseY);

        /* Ticks are in milliseconds, JOYSPEED is in pixels per second. */

        tick = SDL_GetTicks();

        SDL_WarpMouse(MouseX + (int)((x/(float)JOYMAX) 
                                    * (JOYSPEED/1000.0 * (tick - last_tick))),
                     MouseY + (int)((y/(float)JOYMAX) 
                                    * (JOYSPEED/1000.0 * (tick - last_tick))));

        last_tick = tick;
}