void drawImpact(PlayerVisual *pV) {

  glDisable(GL_LIGHTING);
  glPushMatrix();

  glRotatef(90, 90, 0, 1);

  /*
   * Drop impact slightly below the floor and move it a tiny bit
   * back from the impact point so we don't draw behind the
   * trail/wall that's hit.
   */
  glTranslatef(0.0, -0.5, -0.5);

  glColor3f(0.68f, 0, 0);
  
  drawExplosion(&(pV->impact_radius));
  glPopMatrix();
  glEnable(GL_LIGHTING);
}
