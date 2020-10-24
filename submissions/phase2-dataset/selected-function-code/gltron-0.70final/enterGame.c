  updateSettingsCache();

  SystemHidePointer();
  SystemWarpPointer(MOUSE_ORIG_X, MOUSE_ORIG_Y);
  game2->time.offset = SystemGetElapsedTime() - game2->time.current;
  Audio_EnableEngine();
 
	// disable booster and cam offset
	{
		int i;
		for(i = 0; i < game->players; i++) {
			game->player[i].data->boost_enabled = 0;
			game->player[i].camera->movement[CAM_PHI_OFFSET] = 0;
		}
	}

  /* reset pause flag */
  if (game->pauseflag != PAUSE_GAME_FINISHED) {
    game->pauseflag = PAUSE_GAME_RUNNING;
  }
  
  /* fprintf(stderr, "init game\n"); */
}
