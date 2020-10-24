void initCamera(Camera *cam, Data *data, int type) {
	float x,y;
	
	getPositionFromData(&x, &y, data);
	
  cam->type.type = type;

  switch(cam->type.type) {
  case CAM_TYPE_CIRCLING: initCircleCamera(cam); break;
  case CAM_TYPE_FOLLOW: initFollowCamera(cam); break;
  case CAM_TYPE_COCKPIT: initCockpitCamera(cam); break;
  case CAM_TYPE_MOUSE: initFreeCamera(cam); break;
  }
  cam->target[0] = x;
  cam->target[1] = y;
  cam->target[2] = 0;

  cam->cam[0] = x + CAM_CIRCLE_DIST;
  cam->cam[1] = y;
  cam->cam[2] = CAM_CIRCLE_Z;
}
