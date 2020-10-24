	// find distance to enemy walls left & right
	enum { eLeft, eRight, eMax };
	segment2 segments[eMax];

	Data *data = game->player[player].data;
	int dirLeft = (data->dir + 3) % 4;
	int dirRight = (data->dir + 1) % 4;

	float left, right;

	float x, y;
	vec2 vPos;

	int i, j;

	getPositionFromIndex(&x, &y, player);
	vPos.v[0] = x;
	vPos.v[1] = y;

	for(i = 0; i < eMax; i++) {
		vec2Copy(&segments[i].vStart, &vPos);
	}

	segments[eLeft].vDirection.v[0] = dirsX[dirLeft];
	segments[eLeft].vDirection.v[1] = dirsY[dirLeft];
	segments[eRight].vDirection.v[0] = dirsX[dirRight];
	segments[eRight].vDirection.v[1] = dirsY[dirRight];

	left = FLT_MAX;
	right = FLT_MAX;

	for(i = 0; i < game->players; i++) {
		segment2 *wall = game->player[i].data->trails;

		if(i == player)
			continue;
		if(game->player[i].data->trail_height < TRAIL_HEIGHT)
			continue;
		
		for(j = 0; j < game->player[i].data->trailOffset + 1; j++) {
			float t1, t2;
			vec2 v;
			if(segment2_Intersect(&v, &t1, &t2, segments + eLeft, wall) &&
				 t1 > 0 && t1 < left && t2 >= 0 && t2 <= 1)
				left = t1;
			if(segment2_Intersect(&v, &t1, &t2, segments + eRight, wall) &&
				 t1 > 0 && t1 < right && t2 >= 0 && t2 <= 1)
				right = t1;
			wall++;
		}
	}

	{
		float accell_limit = getSettingf("wall_accel_limit");
		if(left < accell_limit || right < accell_limit) {
			float boost = getSettingf("wall_accel_use") * dt / 1000.0f;
			data->speed += boost;
			return 1;
		} else {
			return 0;
		}
	}
}
