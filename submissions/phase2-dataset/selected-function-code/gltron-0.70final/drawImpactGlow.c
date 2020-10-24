
  float opacity;
  
  opacity = GLOW_START_OPACITY - (glow_radius / IMPACT_MAX_RADIUS);
  
  glPushMatrix();
  glScalef(glow_radius, glow_radius, 1.0f);
  
  glBindTexture(GL_TEXTURE_2D, gScreen->textures[TEX_IMPACT]);
  glEnable(GL_TEXTURE_2D);

  glColor4f(GLOW_INTENSITY, GLOW_INTENSITY, GLOW_INTENSITY, opacity);
  glDepthMask(0);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, -1.0);
  glTexCoord2f(1.0, 0.0); glVertex2f(1.0, -1.0);
  glTexCoord2f(1.0, 1.0); glVertex2f(1.0, 1.0);
  glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, 1.0);
  glEnd();
  glDepthMask(1);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}
