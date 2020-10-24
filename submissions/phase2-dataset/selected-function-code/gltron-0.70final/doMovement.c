  int i;
  List *l = NULL;

  for(i = 0; i < game->players; i++) {
		Data *data = game->player[i].data;
		PlayerVisual *pV = gPlayerVisuals + i;
    if(data->speed > 0) { /* still alive */
			float fs;
			float t;

			// speed boost:
			float deccel = 0;
			if(getSettingf("wall_accel_on") == 1) {
				if(!applyWallAcceleration(i, dt)) {
					deccel = getSettingf("wall_accel_decrease");
				}
				else {
					deccel = -1; // forbid deacceleration for booster
				}
			}
			if(getSettingf("booster_on") == 1) {
				if(!applyBooster(i, dt) && deccel != -1) {
					float d = getSettingf("booster_decrease");
					deccel = d > deccel ? d : deccel;
				} else {
					deccel = -1;
				}
			}
			if(deccel > 0)
				applyDecceleration(i, dt, deccel);

			// if(i == 0)
			// printf("speed: %.2f, boost: %.2f\n", data->speed, data->booster);
				
      fs = 1.0f - SPEED_OZ_FACTOR + SPEED_OZ_FACTOR * 
				cosf(i * PI / 4.0f + 
						(game2->time.current % SPEED_OZ_FREQ) * 
						2.0f * PI / SPEED_OZ_FREQ);

      t = dt / 100.0f * data->speed * fs;
			
			{
				segment2 *current = data->trails + data->trailOffset;
				segment2 movement;
				int crash = 0;
				float x, y;
				
				getPositionFromData(&x, &y, data);
				movement.vStart.v[0] = x;
				movement.vStart.v[1] = y;
				movement.vDirection.v[0] = t * dirsX[data->dir];
				movement.vDirection.v[1] = t * dirsY[data->dir];
				
				current->vDirection.v[0] += t * dirsX[data->dir];
				current->vDirection.v[1] += t * dirsY[data->dir];

				crash = crash || crashTestPlayers(i, &movement);
				crash = crash || crashTestWalls(i, &movement);
			}
    } else { /* already crashed */
      if(game2->rules.eraseCrashed == 1 && data->trail_height > 0)
				data->trail_height -= (dt * TRAIL_HEIGHT) / 1000.0f;
      if(pV->exp_radius < EXP_RADIUS_MAX)
				pV->exp_radius += dt * EXP_RADIUS_DELTA;
      else if (data->speed == SPEED_CRASHED) {
				int winner = -1;

				data->speed = SPEED_GONE;
				game->running--;
				if(game->running <= 1) { /* all dead, find survivor */
					int i, maxSpeed = SPEED_GONE;
					/* create winner event */
					for(i = 0; i < game->players; i++) {
						if(game->player[i].data->speed >= maxSpeed) {
							winner = i;
							maxSpeed = game->player[i].data->speed;
						}
					}
					if(mode) {
            createEvent(winner, EVENT_STOP);
						/* a stop event is the last event that happens */
						return l;
					}
				}
      }
    }      
  }
  return l;
}
