void drawTrailLines(Player *p, PlayerVisual *pV) {
  segment2 *s;
  float height;

  float *normal;
  Data *data;
  Camera *cam;

  float trail_top[] = { 1.0, 1.0, 1.0, 1.0 };

  data = p->data;
  cam = p->camera;

  height = data->trail_height;
  if(height <= 0) return;

  /*
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  */

  if (gSettingsCache.antialias_lines) {
    glEnable(GL_LINE_SMOOTH); /* enable line antialiasing */
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_LIGHTING);

  glBegin(GL_LINES);

  s = data->trails;
  while(s != data->trails + data->trailOffset) { 
		/* the current line is not drawn */
    /* compute distance from line to eye point */
    getDist(s, cam->cam);
//		alpha = (game2->rules.grid_size - dist / 2) / game2->rules.grid_size;
    // trail_top[3] = alpha;
    glColor4fv(trail_top);
    
    if(s->vDirection.v[1] == 0) normal = normal1;
    else normal = normal2;
    glNormal3fv(normal);
    glVertex3f(s->vStart.v[0],
							 s->vStart.v[1],
							 height);
    glVertex3f(s->vStart.v[0] + s->vDirection.v[0],
							 s->vStart.v[1] + s->vDirection.v[1],
							 height);
    s++;
    polycount++;
  }
  glEnd();

  /* compute distance from line to eye point */
  getDist(s, cam->cam);
  //alpha = (game2->rules.grid_size - dist / 2) / game2->rules.grid_size;
	// trail_top[3] = alpha;
  glColor4fv(trail_top);

  glBegin(GL_LINES);

	glVertex3f(s->vStart.v[0],
						 s->vStart.v[1],
						 height);
  glVertex3f( getSegmentEndX(data, 0),
	      getSegmentEndY(data, 0),
	      height );

  glEnd();

  /* glEnable(GL_LIGHTING); */
  glDisable(GL_BLEND);
  glDisable(GL_LINE_SMOOTH); /* disable line antialiasing */
  
  /*
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  */
}
