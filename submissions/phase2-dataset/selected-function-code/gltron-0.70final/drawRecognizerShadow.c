void drawRecognizerShadow(void) {
  float dirx;
  vec2 p, v;
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
  getRecognizerPositionVelocity(&p, &v);
  dirx = getRecognizerAngle(&v);

  glPushMatrix();
  glMultMatrixf(shadow_matrix);
  glTranslatef( p.v[0], p.v[1], RECOGNIZER_HEIGHT);
  glRotatef(dirx, 0, 0, 1); /* up */
  glScalef(rec_scale_factor, rec_scale_factor, rec_scale_factor);
  glEnable(GL_NORMALIZE);

  /* render */

  drawModel(recognizer, TRI_MESH);

  /* restore */

  if(gSettingsCache.use_stencil)
    glDisable(GL_STENCIL_TEST);

  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glPopMatrix();
}
