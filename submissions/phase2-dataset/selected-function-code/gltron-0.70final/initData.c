	/* lasttime = SystemGetElapsedTime(); */
	game->pauseflag = PAUSE_GAME_RUNNING;

	game2->rules.speed = getSettingf("speed");
	game2->rules.eraseCrashed = getSettingi("erase_crashed");
	game2->rules.grid_size = getSettingi("grid_size");

	game2->grid.width = game2->rules.grid_size;
	game2->grid.height = game2->rules.grid_size;

	/* time management */
	game2->time.lastFrame = 0;
	game2->time.current = 0;
	game2->time.offset = SystemGetElapsedTime();
	/* TODO: fix that */
	game2->players = game->players;
	/* event management */
	game2->events.next = NULL;
	/* TODO: free any old events that might have gotten left */

  resetVideoData();
	resetPlayerData();

  initWalls();
}
