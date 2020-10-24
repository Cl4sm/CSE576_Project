void doCycleTurnRotation(PlayerVisual *pV, Player *p) {
  int neigung_dir = -1;
	int time = game2->time.current - p->data->turn_time;
  float dirAngle = getDirAngle(time, p);

  glRotatef(dirAngle, 0, 0, 1);

#define neigung 25
  if(time < TURN_LENGTH && p->data->last_dir != p->data->dir) {
    float axis = 1.0f;
    if(p->data->dir < p->data->last_dir && p->data->last_dir != 3)
      axis = -1.0;
    else if((p->data->last_dir == 3 && p->data->dir == 2) ||
	    (p->data->last_dir == 0 && p->data->dir == 3))
      axis = -1.0;
    glRotated(neigung * sin(PI * time / TURN_LENGTH),
	      0.0, axis * neigung_dir, 0.0);
  }
#undef neigung
}
