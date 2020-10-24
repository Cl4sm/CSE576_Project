{
  int i;

	if(state == SYSTEM_KEYSTATE_DOWN) {
		switch (k) {
			/* case 'q': SystemExit(); return; */
		case 27:
			SystemExitLoop(RETURN_GAME_ESCAPE);
			return;
		case ' ':
			SystemExitLoop(RETURN_GAME_PAUSE);
			return;
		case SYSTEM_KEY_F1: changeDisplay(0); return;
		case SYSTEM_KEY_F2: changeDisplay(1); return;
		case SYSTEM_KEY_F3: changeDisplay(2); return;
		case SYSTEM_KEY_F4: changeDisplay(3); return;

		case SYSTEM_KEY_F5: saveSettings(); return;

		case SYSTEM_KEY_F10: nextCameraType(); return;
		case SYSTEM_KEY_F11: doBmpScreenShot(gScreen); return;
		case SYSTEM_KEY_F12: doPngScreenShot(gScreen); return;

		case SYSTEM_KEY_UP: consoleScrollBackward(1); return;
		case SYSTEM_KEY_DOWN: consoleScrollForward(1); return;
    /* toggle lighting
  case SYSTEM_KEY_F6: 
      setSettingi("light_cycles", !game->settings->light_cycles);
      return;
    */
		}
	}
	for( i = 0; i < game->players; i++) {
		if(PLAYER_IS_ACTIVE(&game->player[i]) &&
			 !game->player[i].ai->active) {
			int key;
			if(state == SYSTEM_KEYSTATE_DOWN) { 
				scripting_RunFormat("return settings.keys[%d].left", i + 1);
				scripting_GetIntegerResult( &key );
				if(key == k) {
					createEvent(i, EVENT_TURN_LEFT);
					return;
				}
				scripting_RunFormat("return settings.keys[%d].right", i + 1);
				scripting_GetIntegerResult( &key );
				if(key == k) {
					createEvent(i, EVENT_TURN_RIGHT);
					return;
				}
			}
			// deal with glance keys
			scripting_RunFormat("return settings.keys[%d].glance_left", i + 1);
			scripting_GetIntegerResult( &key );
			if(key == k) {
				if(state == SYSTEM_KEYSTATE_DOWN) {
					printf("glance left down\n");
					game->player[i].camera->movement[CAM_PHI_OFFSET] = PI / 2.0f;
				}	else {
					printf("glance left up\n");
					game->player[i].camera->movement[CAM_PHI_OFFSET] = 0;
				}
				return;
			}
			// deal with glance keys
			scripting_RunFormat("return settings.keys[%d].glance_right", i + 1);
			scripting_GetIntegerResult( &key );
			if(key == k) {
				if(state == SYSTEM_KEYSTATE_DOWN) {
					printf("glance right down\n");
					game->player[i].camera->movement[CAM_PHI_OFFSET] = - PI / 2.0f;
				} else {
					printf("glance right up\n");
					game->player[i].camera->movement[CAM_PHI_OFFSET] = 0;
				}
				return;
			}
			// boost
			scripting_RunFormat("return settings.keys[%d].boost", i + 1);
			scripting_GetIntegerResult( &key );
			if(key == k) {
				if(state == SYSTEM_KEYSTATE_DOWN) {
					printf("boost down\n");
					if(game->player[i].data->booster > getSettingf("booster_min"))
						game->player[i].data->boost_enabled = 1;
				} else {
					printf("boost up\n");
					game->player[i].data->boost_enabled = 0;
				}
				return;
			}
		}
	}
	if(state == SYSTEM_KEYSTATE_DOWN) {
		displayMessage(TO_STDERR, "key '%s' (%d) is not bound", 
									 SystemGetKeyName(k), k);
	}
}
