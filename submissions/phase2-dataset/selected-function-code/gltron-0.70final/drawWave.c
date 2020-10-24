static void drawWave(double radius) {
  int i, j;
  double angle;
  double delta_radius = SHOCKWAVE_WIDTH / SHOCKWAVE_SEGMENTS;
  double delta_angle = (180.0 / SHOCKWAVE_SEGMENTS) * (PI/180);
  double start_angle = 270.0 * (PI/180);
  
  for (i = 0; i < SHOCKWAVE_SEGMENTS; i++) {
    glBegin(GL_QUAD_STRIP);
    angle = start_angle;
    for (j = 0; j <= SHOCKWAVE_SEGMENTS; j++) {
      glVertex2d((radius + delta_radius) * sin(angle),
          (radius + delta_radius) * cos(angle));
      glVertex2d(radius * sin(angle), radius * cos(angle));
      angle += delta_angle;
    }
    glEnd();
    radius += delta_radius;
  }
}
