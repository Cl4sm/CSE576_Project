void ai_getClosestOpponent(int player, int* opponent, float *distance) {
	int i;
	vec2 v_player;
	vec2 v_opponent;

	*opponent = -1;
	*distance = FLT_MAX;

	getPositionFromIndex(v_player.v + 0, v_player.v + 1, player);
	
	for(i = 0; i < game->players; i++) {
		if(i == player)
			continue;
		if(game->player[i].data->speed > 0) {
			vec2 diff;
			float d;

			getPositionFromIndex(v_opponent.v + 0, v_opponent.v + 1, i);
			vec2Sub(&diff, &v_player, &v_opponent);
			// use manhattan distance instead of euclidean distance
			d = fabs(diff.v[0]) + fabs(diff.v[1]);
			// d = vec2Length(&diff);
			if(d < *distance) {
				*opponent = i;
				*distance = d;
			}
		}
	}
} 
