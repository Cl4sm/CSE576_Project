void Game_Idle(void) {
  List *l;
  List *p;
  int i;
  int dt;
  int t;

	switch(game2->mode) {
	case GAME_SINGLE:
#ifdef RECORD
	case GAME_SINGLE_RECORD:
#endif
		/* check for fast finish */
    
		if (gSettingsCache.fast_finish == 1) {
			int factors[4] = { 4, 6, 12, 25 };
			int threshold[4] = { 0, 300, 600, 800 };
			int factor = 1;
			for(i = 0; i < 4; i++) {
				if(game2->rules.grid_size > threshold[i])
					factor = factors[i];
			}
			for (i = 0; i < game->players; i++) {
				if (game->player[i].ai->active != AI_COMPUTER &&
						gPlayerVisuals[i].exp_radius < EXP_RADIUS_MAX) {
					factor = 1;
				}
			}
			dt = game2->time.dt * factor;
		} else { 
			dt = game2->time.dt;
		}

		while(dt > 0) {
			if(dt > PHYSICS_RATE) t = PHYSICS_RATE;
			else t = dt;

			/* run AI */
			for(i = 0; i < game->players; i++)
				if(game->player[i].ai != NULL)
					if(game->player[i].ai->active == AI_COMPUTER &&
						 PLAYER_IS_ACTIVE(&game->player[i])) {
						doComputer(i, 0);
					}

			/* process any outstanding events (turns, etc) */
			for(p = &(game2->events); p->next != NULL; p = p->next) {
				if(processEvent((GameEvent*) p->data)) return;
			}

			/* free events */
			p = game2->events.next;
			while(p != NULL) {
				l = p;
				p = p->next;
				free(l);
			}
			game2->events.next = NULL;

			l = doMovement(1, t); /* this can generate new events */
			if(l != NULL) {
				for(p = l; p->next != NULL; p = p->next) {
					(void) processEvent((GameEvent*) p->data);
				}

			}
			/* free list  */
			p = l;
			while(p != NULL) {
				l = p;
				p = p->next;
				free(l);
			}
			dt -= PHYSICS_RATE;
		}
		break;
#ifdef RECORD
	case GAME_PLAY_NETWORK:
		/* fall through to GAME_PLAY */
	case GAME_PLAY:
		getEvents(); 
		l = doMovement(0, game2->time.dt); /* this won't generate new events */
		if(l != NULL) {
			fprintf(stderr, "something is seriously wrong - ignoring events\n");
		}
		break;
#endif /* RECORD */
	}
    
	doCameraMovement();
	doRecognizerMovement();
}
