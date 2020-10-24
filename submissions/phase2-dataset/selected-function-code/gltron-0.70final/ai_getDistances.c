void ai_getDistances(int player, AI_Distances *distances) {
	enum { eFront = 0, eLeft, eRight, eBackleft, eMax };
	segment2 segments[eMax];
	vec2 v, vPos;
	Data *data = game->player[player].data;
	int dirLeft = (data->dir + 3) % 4;
	int dirRight = (data->dir + 1) % 4;
	int i, j;
	float *front = &distances->front;
	float *right = &distances->right;
	float *left = &distances->left;
	float *backleft = &distances->backleft;

	getPositionFromIndex(vPos.v + 0, vPos.v + 1, player);

	for(i = 0; i < eMax; i++) {
		vec2Copy(&segments[i].vStart, &vPos);
	}

	segments[eFront].vDirection.v[0] = dirsX[data->dir];
	segments[eFront].vDirection.v[1] = dirsY[data->dir];
	segments[eLeft].vDirection.v[0] = dirsX[dirLeft];
	segments[eLeft].vDirection.v[1] = dirsY[dirLeft];
	segments[eRight].vDirection.v[0] = dirsX[dirRight];
	segments[eRight].vDirection.v[1] = dirsY[dirRight];
	segments[eBackleft].vDirection.v[0] = dirsX[dirLeft] - dirsX[data->dir];
	segments[eBackleft].vDirection.v[1] = dirsY[dirLeft] - dirsY[data->dir];
	vec2Normalize(&segments[eBackleft].vDirection,
								&segments[eBackleft].vDirection);
	*front = FLT_MAX;
	*left = FLT_MAX;
	*right = FLT_MAX;
	*backleft = FLT_MAX;

	// loop over all segment
	for(i = 0; i < game->players; i++) {
		segment2 *wall = game->player[i].data->trails;
		if(game->player[i].data->trail_height < TRAIL_HEIGHT)
			continue;

		for(j = 0; j < game->player[i].data->trailOffset + 1; j++) {
			float t1, t2;
			if(i == player && j == game->player[i].data->trailOffset)
				break;
			if(segment2_Intersect(&v, &t1, &t2, segments + eFront, wall) &&
				 t1 > 0 && t1 < *front && t2 >= 0 && t2 <= 1)
				*front = t1;
			if(segment2_Intersect(&v, &t1, &t2, segments + eLeft, wall) &&
				 t1 > 0 && t1 < *left && t2 >= 0 && t2 <= 1)
				*left = t1;
			if(segment2_Intersect(&v, &t1, &t2, segments + eRight, wall) &&
				 t1 > 0 && t1 < *right && t2 >= 0 && t2 <= 1)
				*right = t1;
			if(segment2_Intersect(&v, &t1, &t2, segments + eBackleft, wall) &&
				 t1 > 0 && t1 < *backleft && t2 >= 0 && t2 <= 1)
				*backleft = t1;
			wall++;
		}
	}
	for(i = 0; i < 4; i++) {
		float t1, t2;
		segment2* wall = walls + i;
		if(segment2_Intersect(&v, &t1, &t2, segments + eFront, wall) &&
			 t1 > 0 && t1 < *front && t2 >= 0 && t2 <= 1)
			*front = t1;
		if(segment2_Intersect(&v, &t1, &t2, segments + eLeft, wall) &&
			 t1 > 0 && t1 < *left && t2 >= 0 && t2 <= 1)
			*left = t1;
		if(segment2_Intersect(&v, &t1, &t2, segments + eRight, wall) &&
			 t1 > 0 && t1 < *right && t2 >= 0 && t2 <= 1)
			*right = t1;
		if(segment2_Intersect(&v, &t1, &t2, segments + eBackleft, wall) &&
			 t1 > 0 && t1 < *backleft && t2 >= 0 && t2 <= 1)
			*backleft = t1;
	}
	
	// update debug render segments
	{
		AI *ai = game->player[player].ai;
		vec2Copy(&ai->front.vStart, &vPos);
		vec2Copy(&ai->left.vStart, &vPos);
		vec2Copy(&ai->right.vStart, &vPos);
		vec2Copy(&ai->backleft.vStart, &vPos);
		
		ai->front.vDirection.v[0] = *front * dirsX[data->dir];
		ai->front.vDirection.v[1] = *front * dirsY[data->dir];
		ai->left.vDirection.v[0] = *left * dirsX[dirLeft];
		ai->left.vDirection.v[1] = *left * dirsY[dirLeft];
		ai->right.vDirection.v[0] = *right * dirsX[dirRight];
		ai->right.vDirection.v[1] = *right * dirsY[dirRight];
		ai->backleft.vDirection.v[0] = dirsX[dirLeft] - dirsX[data->dir];
		ai->backleft.vDirection.v[1] = dirsY[dirLeft] - dirsY[data->dir];
		vec2Normalize(&ai->backleft.vDirection,
									&ai->backleft.vDirection);
		vec2Scale(&ai->backleft.vDirection, 
							&ai->backleft.vDirection,
							*backleft);
	}
		
	// printf("%.2f, %.2f, %.2f\n", *front, *right, *left);
	return;
}
