                   float line_color[4], float square_color[4]) {
  int i, j;

  glColor3fv(line_color);
  
  glFogfv(GL_FOG_COLOR, square_color);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogi(GL_FOG_START, 100);
  glFogi(GL_FOG_END, 350);

  glEnable(GL_FOG);

  glBegin(GL_LINES);
  for (i = 0; i < grid_size; i += line_spacing) {
    for (j = 0; j < grid_size; j += line_spacing) {
      glVertex3i(i, j, 0);
      glVertex3i(i + line_spacing, j, 0);
      glVertex3i(i, j, 0);
      glVertex3i(i, j + line_spacing, 0);
    }
  }
  glEnd();

  glDisable(GL_FOG);
}
