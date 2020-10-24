void drawGuiBackground(void) {
  checkGLError("gui background start");

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rasonly(gScreen);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gScreen->textures[TEX_GUI]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);

  glTexCoord2f(0.0, 0.0);
  glVertex2f(0, 0);

  glTexCoord2f(1.0, 0.0);
  glVertex2i(gScreen->vp_w, 0);

  glTexCoord2f(1.0, .75);
  glVertex2i(gScreen->vp_w, gScreen->vp_h);

  glTexCoord2f(0.0, .75);
  glVertex2i(0, gScreen->vp_h);

  glEnd();
}
