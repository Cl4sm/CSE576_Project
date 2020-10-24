void doComputerSimple(int player, int target, AI_Distances *distances) {
  AI *ai;
  Data *data;
  Player *me;
  int level;

  me = &(game->player[ player ]);
  if(me->ai == NULL) {
    printf("This player has no AI data!\n");
    return;
  }
  data = me->data;
  ai = me->ai;
  level = gSettingsCache.ai_level;

	// turn off booster
	data->boost_enabled = 0;

  /* avoid too short turns */
  if(game2->time.current - ai->lasttime < ai_params.minTurnTime[level])
    return;

	// ai_getDistances(player, &front, &left, &right, &backleft);

	/* first, check if we are in danger, or if we should consider
		 turning once in a while
	 */

  if(distances->front > ai_params.critical[level] * game2->rules.grid_size &&
		 segment2_Length(data->trails + data->trailOffset) <
		 ai_params.maxSegLength[level] * game2->rules.grid_size)
		return;
		
	// printf("%.2f, %.2f, %.2f\n", distances->front, 
	// distances->left, distances->right);
	/* decide where to turn */
	if(distances->front > distances->right && 
		 distances->front > distances->left) { 
		return; /* no way out */
	} else if(distances->left > ai_params.rlDelta[level] &&
						abs(distances->right - distances->left) < 
						ai_params.rlDelta[level] &&
						distances->backleft > distances->left &&
						ai->tdiff < ai_params.spiral[level]) {
		createEvent(player, EVENT_TURN_LEFT);
		ai->tdiff++;
	} else if(distances->right > distances->left && 
						ai->tdiff > - ai_params.spiral[level] ) {
		createEvent(player, EVENT_TURN_RIGHT);
		ai->tdiff--;
		// printf("turn right\n");
	} else if(distances->right < distances->left && 
						ai->tdiff < ai_params.spiral[level] ) {
		createEvent(player, EVENT_TURN_LEFT);
		ai->tdiff++;
		// printf("turn left\n");
	} else {
		printf("tdiff decision\n");
		if(ai->tdiff > 0) { 
			createEvent(player, EVENT_TURN_RIGHT);
			ai->tdiff--; 
			// printf("turn right\n");
		} else { 
			createEvent(player, EVENT_TURN_LEFT);
			ai->tdiff++; 
			// printf("turn left\n");
		}
	}
	ai->lasttime = game2->time.current;
}
