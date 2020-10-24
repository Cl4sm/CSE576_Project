  Mesh *cycle;
  int turn_time = game2->time.current - p->data->turn_time;
      
  if(turn_time < TURN_LENGTH && !drawTurn)
    return;

  if(pV->exp_radius != 0)
    return;

  cycle = lightcycle[lod];

  /* states */

  glEnable(GL_CULL_FACE);

  if(gSettingsCache.use_stencil) {
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_GREATER, 1, 1);
    glEnable(GL_BLEND);
    glColor4fv(shadow_color);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glColor3f(0, 0, 0);
    glDisable(GL_BLEND);
  }

  /* transformations */

  glPushMatrix();
	{
		float x, y;
		getPositionFromData(&x, &y, p->data);
		glTranslatef(x,y, 0.0);
	}
  glMultMatrixf(shadow_matrix);
  if (gSettingsCache.turn_cycle) {
    doCycleTurnRotation(pV, p);
  } else if (pV->exp_radius == 0) {
    glRotatef(dirangles[p->data->dir], 0.0, 0.0, 1.0);
  }
  glTranslatef(0, 0, cycle->BBox.vSize.v[2] / 2);

  /* render */

  drawModel(cycle, TRI_MESH);

  /* restore */

  if(gSettingsCache.use_stencil)
    glDisable(GL_STENCIL_TEST);

  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glPopMatrix();
}
