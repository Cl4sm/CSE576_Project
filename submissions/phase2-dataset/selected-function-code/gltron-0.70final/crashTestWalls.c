	int j;
	vec2 v;
	float t1, t2;
	int crash = 0;

	Data *data = game->player[i].data;
	segment2 *current = data->trails + data->trailOffset;
	
	for(j = 0; j < 4; j++) {
		if(segment2_Intersect(&v, &t1, &t2, current, walls + j)) {
			if(t1 >= 0 && t1 < 1 && t2 >= 0 && t2 < 1) {
				current->vDirection.v[0] = v.v[0] - current->vStart.v[0];
				current->vDirection.v[1] = v.v[1] - current->vStart.v[1];
				createEvent(i, EVENT_CRASH);
				crash = 1;
				break;
			}
		}
	}
	return crash;
}
