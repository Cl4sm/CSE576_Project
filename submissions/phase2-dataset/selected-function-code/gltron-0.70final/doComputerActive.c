void doComputerActive(int player, int target, AI_Distances *distances) {
	Data *data = game->player[player].data;
	AI *ai = game->player[player].ai;

	AI_Configuration config;

  if(game2->time.current - ai->lasttime < 200)
    return;

	ai_getConfig(player, target, &config);
	
	// who's going to reach the intersection point first?
	// printf("active, location %d (direction: %d)\n", 
	// config.location, data->dir);
	
	switch(config.location) {
	case 0: case 7: 
	case 1: case 6:
		if(config.t_player < config.t_opponent) {
			// printf("I'm ahead: %.2f, %.2f\n", config.t_player, config.t_opponent);
			if(config.t_player - config.t_opponent < SAVE_T_DIFF)
				data->boost_enabled = 1;
			if(game->player[player].data->speed -
				 game->player[target].data->speed < SAVE_SPEED_DIFF)
				data->boost_enabled = 1;
			// aggressive
			ai_aggressive(player, target, config.location, distances);
		} else {
			// printf("I'm behind: %.2f, %.2f\n", 
			// config.t_player, config.t_opponent);
			// check speed, adjust or evasive
			if(config.t_opponent < HOPELESS_T) {
				// evasive
				ai_evasive(player, target, config.location, distances);
			}	else if(config.t_opponent - config.t_player < SAVE_T_DIFF)	{
				data->boost_enabled = 1;
				// aggressive
				ai_aggressive(player, target, config.location, distances);
			}	else {
				// evasive
				ai_evasive(player, target, config.location, distances);
			}
		}
		break;
	case 2:	case 4:
	case 3:	case 5:
		doComputerSimple(player, target, distances);
		break;
	}
}
