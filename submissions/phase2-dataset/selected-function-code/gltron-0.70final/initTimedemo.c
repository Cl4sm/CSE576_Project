void initTimedemo(void) {
	int i = 0;

	printf("-- initializing timedemo\n");
	
	frames = 0;
	startTime = SystemGetElapsedTime();
	
	tsrand(12313);

	resetRecognizer();
	
	updateSettingsCache();

	// overwrite AI skills & rules in settingsCache
	gSettingsCache.ai_level = 2;
	gSettingsCache.show_ai_status = 0;
	gSettingsCache.show_fps = 0;
	gSettingsCache.camType = CAM_CIRCLE;
	gSettingsCache.show_console = 0;
	
	saveRules.speed = getSettingf("speed");
	saveRules.eraseCrashed = getSettingi("erase_crashed");
	saveRules.grid_size = getSettingi("grid_size");

  setSettingf("speed", 12);
	setSettingi("erase_crashed", 1);
	setSettingi("grid_size", 200);
		
 	game2->mode = GAME_SINGLE;
  initData();
  changeDisplay(-1);

	for(i = 0; i < game->players; i++) {
		game->player[i].ai->active = AI_COMPUTER;
		// set all camera phi values to 0
		game->player[i].camera->movement[CAM_PHI] = PI / 18;
		game->player[i].camera->movement[CAM_CHI] = PI / 3;
	}
	
  SystemHidePointer();
  SystemWarpPointer(MOUSE_ORIG_X, MOUSE_ORIG_Y);
  game2->time.offset = SystemGetElapsedTime() - game2->time.current;
}
