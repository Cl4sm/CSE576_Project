void ftxRenderString(FontTex *ftx, const char *string, int len) {
  int i;
  int bound = -1;
  int index;
  
  int tex;
  int w;
  float cw;
  float cx, cy;

  w = ftx->texwidth / ftx->width;
  cw = (float)ftx->width / (float)ftx->texwidth;

  for(i = 0; i < len; i++) {
    if(string[i] == 3) { /* color code */
      i++;
      if(i >= len) return;
      if(string[i] < color_base && string[i] > color_base + colors) continue;
      glColor3ubv(color_codes[ string[i] - color_base ]);
      continue;
    }
      
    /* find out which texture it's in */
    /* TODO(4): find out why the +1 is necessary */
    index = string[i] - ftx->lower + 1;
    if(index >= ftx->upper) 
      fprintf(stderr, FTX_ERR " index out of bounds");
    tex = index / (w * 
w);
    /* bind texture */
    if(tex != bound) {
      glBindTexture(GL_TEXTURE_2D, ftx->texID[tex]);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      bound = tex;
    }
    /* find texture coordinates */
    index = index % (w * w);
    cx = (float)(index % w) / (float)w;
    cy = (float)(index / w) / (float)w;
    /* draw quad */
    /* fprintf(stderr, FTX_ERR "coords: tex %d (%.2f, %.2f), %.2f\n", */
    /*     bound, cx, cy, cw); */

    glBegin(GL_QUADS);
    glTexCoord2f(cx, 1 - cy - cw);
    glVertex2f(i, 0);
    glTexCoord2f(cx + cw, 1 - cy - cw);
    glVertex2f(i + 1, 0);
    glTexCoord2f(cx + cw, 1 - cy);
    glVertex2f(i + 1, 1);
    glTexCoord2f(cx, 1 - cy);
    glVertex2f(i, 1);
    glEnd();
  }
  /* checkGLError("FontTex.c ftxRenderString\n"); */
}
