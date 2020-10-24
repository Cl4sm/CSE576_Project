	/* keyboard */
  SDL_EnableKeyRepeat(0, 0); /* turn keyrepeat off */
  
	/* joystick */
	if(SDL_Init(SDL_INIT_JOYSTICK) >= 0) {
		int i;
		int joysticks = SDL_NumJoysticks();

		/* FIXME: why only two joysticks? */
		/* joystick, currently at most 2 */
		if(joysticks > 2)
			joysticks = 2;
		
		for(i = 0; i < joysticks; i++) {
			SDL_JoystickOpen(i);
		}
		if(i)
			SDL_JoystickEventState(SDL_ENABLE);
	} else {
		const char *s = SDL_GetError();
		fprintf(stderr, "[init] couldn't initialize joysticks: %s\n", s);
	}
}
