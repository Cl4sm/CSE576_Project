  int i;
  Data *data;
  AI *ai;
  int not_playing = 0;

  int *startIndex;
  startIndex = malloc( game->players * sizeof(int) );
  randomPermutation(game->players, startIndex);

  for(i = 0; i < game->players; i++) {
    float startpos[][2] = { 
			{ 0.5, 0.25 }, { 0.75, 0.5 }, { 0.5, 0.75 }, { 0.25, 0.5 }
    };
		float x, y;

    data = game->player[i].data;
    ai = game->player[i].ai;
    /* init ai */

		switch(i) {
		case 0: ai->active = getSettingi("ai_player1"); break;
		case 1: ai->active = getSettingi("ai_player2"); break;
		case 2: ai->active = getSettingi("ai_player3"); break;
		case 3: ai->active = getSettingi("ai_player4"); break;
		default:
			fprintf(stderr, "[error] player index #%d not caught!\n", i);
			ai->active = AI_NONE;
		}
		ai->tdiff = 0;

		/* arrange players in circle around center */

		/* randomize position on the grid */
		x = startpos[ startIndex[i] ][0] * getSettingi("grid_size");
		y = startpos[ startIndex[i] ][1] * getSettingi("grid_size");
		/* randomize starting direction */
		data->dir = trand() & 3;
		/* data->dir = startdir[i]; */
		data->last_dir = data->dir;

		/* if player is playing... */
		if(ai->active != AI_NONE) {
			data->speed = getSettingf("speed");
			data->booster = getSettingf("booster_max");
			data->boost_enabled = 0;
			data->trail_height = TRAIL_HEIGHT;
		} else {
			data->speed = SPEED_GONE;
			data->trail_height = 0;
			not_playing++;
		}
		// data->trail = data->trails;
		data->trailOffset = 0;

		data->trails[ data->trailOffset ].vStart.v[0] = x;
		data->trails[ data->trailOffset ].vStart.v[1] = y;
		
		data->trails[ data->trailOffset ].vDirection.v[0] = 0;
		data->trails[ data->trailOffset ].vDirection.v[1] = 0;

		{
			int camType;
			Camera *cam = game->player[i].camera;
			camType = (game->player[i].ai->active == AI_COMPUTER) ? 
				CAM_CIRCLE : gSettingsCache.camType;
			initCamera(cam, data, camType);
		}
	}

	free(startIndex);

	game->running = game->players - not_playing; /* not everyone is alive */
	/* printf("starting game with %d players\n", game->running); */
	game->winner = -1;
}
