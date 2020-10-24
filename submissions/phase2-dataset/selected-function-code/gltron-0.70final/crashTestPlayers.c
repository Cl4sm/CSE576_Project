	int j, k;
	int crash = 0;
	Data *data = game->player[i].data;
	segment2 *current = data->trails + data->trailOffset;
	// debug: only test player0 against himself
	// j = 0; 
	// if(i == 0) { 
	for(j = 0; j < game->players; j++) {
		int crash = 0;

		if(game->player[j].data->trail_height < TRAIL_HEIGHT)
			continue;

		for(k = 0; k < game->player[j].data->trailOffset + 1; k++) {
			segment2 *wall;
			vec2 v;
			float t1, t2;
						
			if(j == i && k >= game->player[j].data->trailOffset - 1)
				break;

			wall = game->player[j].data->trails + k;
						
			if(segment2_Intersect(&v, &t1, &t2, movement, wall)) {
#if 0
				printf("(%.2f, %.2f), (%.2f, %.2f), %.2f, %.2f\n",
							 data->posx, data->posy,
							 v.v[0], v.v[1],
							 t1, t2); 
#endif
				if(t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
					current->vDirection.v[0] = v.v[0] - current->vStart.v[0];
					current->vDirection.v[1] = v.v[1] - current->vStart.v[1];
					createEvent(i, EVENT_CRASH);
					crash = 1;
					break;
				}
			}
		}
		if(crash)
			break;
	}
	return crash;
}
