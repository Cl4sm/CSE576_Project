void ai_getConfig(int player, int target,
										AI_Configuration *config) {

	Data *data;
	
	getPositionFromIndex(config->player.vStart.v + 0,
											 config->player.vStart.v + 1,
											 player);
	getPositionFromIndex(config->opponent.vStart.v + 0,
											 config->opponent.vStart.v + 1,
											 target);
	
	data = game->player[player].data;
	config->player.vDirection.v[0] = dirsX[ data->dir ] * data->speed;
	config->player.vDirection.v[1] = dirsY[ data->dir ] * data->speed;

	data = game->player[target].data;
	config->opponent.vDirection.v[0] = dirsX[ data->dir ] * data->speed;
	config->opponent.vDirection.v[1] = dirsY[ data->dir ] * data->speed;
	
	// compute sector
	{
		vec2 diff;
		vec3 v1, v2, v3;
		vec3 up = { { 0, 0, 1 } };
		float cosphi;
		float phi;
		int i;

		vec2Sub(&diff, &config->player.vStart, &config->opponent.vStart);
		v1.v[0] = diff.v[0];
		v1.v[1] = diff.v[1];
		v1.v[2] = 0;

		v2.v[0] = config->opponent.vDirection.v[0];
		v2.v[1] = config->opponent.vDirection.v[1];
		v2.v[2] = 0;

		vec3Normalize(&v1, &v1);
		vec3Normalize(&v2, &v2);

		vec3Cross(&v3, &v1, &v2);
		vec3Normalize(&v3, &v3);
	
		cosphi = vec3Dot(&v1, &v2);
		clamp(&cosphi, -1, 1);
		phi = acos(cosphi);
		if(vec3Dot(&v3, &up) > 0)
			phi = 2 * M_PI - phi;
	
		for(i = 0; i < 8; i++) {
			phi -= M_PI / 4;
			if(phi < 0) {
				config->location = i;
				break;
			}
		}
	}
	// compute intersection
	{
		segment2 seg1;
		segment2 seg2;
		seg1.vStart = config->opponent.vStart;
		seg1.vDirection = config->opponent.vDirection;
		seg2.vStart = config->player.vStart;
		vec2_Orthogonal( &seg2.vDirection, &config->opponent.vDirection );
		vec2Normalize( &seg2.vDirection, &seg2.vDirection );
		vec2Scale( &seg2.vDirection, 
							 &seg2.vDirection,
							 vec2Length( &config->player.vDirection )
							 );
							 
		segment2_Intersect( &config->intersection, 
												&config->t_opponent, &config->t_player,
												&seg1, &seg2 );
		if(config->t_player < 0)
			config->t_player *= -1;
	}
	
}
