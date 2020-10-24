void initWalls(void) {
	float raw[4][4] = {
		{ 0.0f, 0.0f, 1.0f,  0.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, -1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, -1.0f }
	};

	float width = game2->rules.grid_size;
	float height = game2->rules.grid_size;
	
	int j;

	for(j = 0; j < 4; j++) {
		walls[j].vStart.v[0] = raw[j][0] * width;
		walls[j].vStart.v[1] = raw[j][1] * height;
		walls[j].vDirection.v[0] = raw[j][2] * width;
		walls[j].vDirection.v[1] = raw[j][3] * height;
	}
}
