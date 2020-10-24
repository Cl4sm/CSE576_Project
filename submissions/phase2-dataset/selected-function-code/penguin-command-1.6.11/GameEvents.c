int GameEvents(const SDL_Event *event)
{
    int button=-1;
    Uint8* keys;

    switch (event->type) {
	case SDL_QUIT:
	    printf("Quit Requested\n");
	    Quit=1;
	break;
#ifdef JOYSTICK
        /* JOYAXISMOTION is handled elsewhere. */
        case SDL_JOYBUTTONDOWN:
                /* TODO: Let user configure which button does what. */
                if (event->jbutton.state == SDL_PRESSED) {
                        SDL_GetMouseState(&MouseX, &MouseY);
                        /* Joystick buttons are numbered from 0, mouse
                          buttons are numbered from 1 */
                        CreateShot(MouseX,MouseY,event->button.button+1);
                }
                break;
#endif /* JOYSTICK */	
	case SDL_MOUSEBUTTONDOWN:
	    SDL_GetMouseState(&MouseX, &MouseY);
    	    CreateShot(MouseX,MouseY,event->button.button);
	break;    
	case SDL_KEYDOWN:
	    keys = SDL_GetKeyState(NULL);
    	    if ( event->key.keysym.sym == SDLK_a ) button=1;
    	    if ( event->key.keysym.sym == SDLK_s ) button=2;
	    if ( event->key.keysym.sym == SDLK_d ) button=3;
	    if ( button > 0 ) {
		CreateShot(MouseX,MouseY,button);
	    }
	    if ( event->key.keysym.sym == SDLK_ESCAPE ) {
		printf("Quit Requested\n");
		endgame=1;
	    }
            // Fullscreen toggle
	    if ( (event->key.keysym.sym == SDLK_RETURN) &&
		(event->key.keysym.mod & KMOD_ALT) )
		    SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
	    // Pause
	    if (( ( event->key.keysym.sym == SDLK_PAUSE ) ||
	        ( event->key.keysym.sym == SDLK_p ) ) &&
		( !pause) )
		    Pause(event);
	break;
    }    
    return 1;

}