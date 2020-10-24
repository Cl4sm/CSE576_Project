void drawCredits(void) {
  int time;
  int x, y;
  int h;
  int i;
  float colors[][3] = { { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } };
  time = SystemGetElapsedTime() - coffset;

  glClearColor(.0, .0, .0, .0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rasonly(gScreen);
  h = gScreen->vp_h / (24 * 3 / 2);
  for(i = 0; i < time / 250; i++) {
    glColor3fv(colors[i % 2]);
    if(credits[i] == NULL) 
      break;
    x = 10;
    y = gScreen->vp_h - 3 * h * (i + 1) / 2;
    drawText(gameFtx, x, y, h, credits[i]);
  }
}
