int drawFloorTextured(int grid_size, GLuint texture) {
  int i, j, l, t;
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

    /* there are some strange clipping artefacts on some renderers */
    /* try subdividing things... */

    glColor4f(1.0, 1.0, 1.0, 1.0);
    l = grid_size / 4;
    t = l / 12;
    
    //  glBegin(GL_QUADS);
    for (i = 0; i < grid_size; i += l) {
      glBegin(GL_QUADS);
      for (j = 0; j < grid_size; j += l) {
        glTexCoord2i(0, 0);
        glVertex2i(i, j);
        glTexCoord2i(t, 0);
        glVertex2i(i + l, j);
        glTexCoord2i(t, t);
        glVertex2i(i + l, j + l);
        glTexCoord2i(0, t);
        glVertex2i(i, j + l);
      }
      glEnd();
    }
    // glEnd();
   
    glDisable(GL_TEXTURE_2D);
    return grid_size * grid_size;
}
