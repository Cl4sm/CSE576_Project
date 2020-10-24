  float dest[3];
  float tdest[3];
  float phi, chi, r;
	float x, y;
	Camera *cam;
	Data *data;
  /* first, process all movement commands */
  /* that means, check for mouse input mainly */

  /* dt hack: the time since the last frame is not necessarily the game
     time, since the game maybe hasn't started yet, or was paused */
  static int last=0;
  int dt;

	if(game2->time.dt == 0) {
		dt = SystemGetElapsedTime() - last;
		last = SystemGetElapsedTime();
	} else {
		dt = game2->time.dt;
	}

	cam = p->camera;
	data = p->data;
	getPositionFromData(&x, &y, data);

  if(cam->type.freedom[CAM_FREE_R]) {
    if(gInput.mouse1 == 1)
      cam->movement[CAM_R] += (cam->movement[CAM_R]-CLAMP_R_MIN+1) * dt / 300.0f;
    if(gInput.mouse2 == 1)
      cam->movement[CAM_R] -= (cam->movement[CAM_R]-CLAMP_R_MIN+1) * dt / 300.0f;
    writeCamDefaults(cam, CAM_R);
  }

  if(cam->type.freedom[CAM_FREE_PHI]) {
    cam->movement[CAM_PHI] += - gInput.mousex * MOUSE_CX;
    writeCamDefaults(cam, CAM_CHI);
  }
  if(cam->type.freedom[CAM_FREE_CHI]) {
    cam->movement[CAM_CHI] += gInput.mousey * MOUSE_CY;
    writeCamDefaults(cam, CAM_PHI);
  }
  /* done with mouse movement, now clamp the camera to legal values */
  clampCam(cam);

  phi = cam->movement[CAM_PHI] + cam->movement[CAM_PHI_OFFSET];
  chi = cam->movement[CAM_CHI];
  r = cam->movement[CAM_R];

  /* if the cam is coupled to player movement, change the phi accordingly */
  if(cam->type.coupled) {
    int time = game2->time.current - p->data->turn_time;
    if(time < TURN_LENGTH) {
      int dir, ldir;
      dir = p->data->dir;
      ldir = p->data->last_dir;
      if(dir == 1 && ldir == 2)
	dir = 4;
      if(dir == 2 && ldir == 1)
	ldir = 4;
      phi += ((TURN_LENGTH - time) * camAngles[ldir] + 
	      time * camAngles[dir]) / TURN_LENGTH;
    }
    else
      phi += camAngles[p->data->dir];
  }

  /* position the camera */
  dest[0] = x + r * cosf(phi) * sinf(chi);
  dest[1] = y + r * sinf(phi) * sinf(chi);
  dest[2] = r * cosf(chi);

  /* ok, now let's calculate the new camera destination coordinates */
  /* also, perform some camera dependant movement */
  switch(cam->type.type) {
  case CAM_TYPE_CIRCLING: /* Andi-cam */
    cam->movement[CAM_PHI] += CAM_SPEED * game2->time.dt;
    tdest[0] = x;
    tdest[1] = y;
    tdest[2] = B_HEIGHT;
    break;
  case CAM_TYPE_FOLLOW: /* Mike-cam */
    tdest[0] = x;
    tdest[1] = y;
    tdest[2] = B_HEIGHT;
    break;
  case CAM_TYPE_COCKPIT: /* 1st person */
    tdest[0] = x + 4.0f * dirsX[ p->data->dir ] + 2.0f * cosf(phi);
    tdest[1] = y + 4.0f * dirsY[ p->data->dir ] + 2.0f * sinf(phi);
    tdest[2] = CAM_COCKPIT_Z;
    dest[0] = x + 4.0f * dirsX[ p->data->dir ] + 0.1f * cosf(phi);
    dest[1] = y + 4.0f * dirsY[ p->data->dir ] + 0.1f * sinf(phi);
    dest[2] = CAM_COCKPIT_Z + 0.1f;
    break;
  case CAM_TYPE_MOUSE: /* mouse camera */
    tdest[0] = x;
    tdest[1] = y;
    tdest[2] = B_HEIGHT;
    break;
  }
  memcpy(cam->cam, dest, sizeof(cam->cam));
  memcpy(cam->target, tdest, sizeof(cam->target));
}
