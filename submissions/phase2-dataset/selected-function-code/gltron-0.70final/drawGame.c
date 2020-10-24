  GLint i;

  polycount = 0;

  glEnable(GL_DEPTH_TEST);

  glClearColor(gSettingsCache.clear_color[0], 
               gSettingsCache.clear_color[1], 
               gSettingsCache.clear_color[2],
               gSettingsCache.clear_color[3]);

  if(gSettingsCache.use_stencil) {
    glClearStencil(0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
  } else {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  }

  for(i = 0; i < vp_max[gViewportType]; i++) {
		Player *p = game->player + viewport_content[i];
		PlayerVisual *pV = gPlayerVisuals + viewport_content[i];
		Visual *d = & pV->display;
	
    if(d->onScreen == 1) {
      glViewport(d->vp_x, d->vp_y, d->vp_w, d->vp_h);
			drawCam(p, pV);
      glDisable(GL_DEPTH_TEST);
      glDepthMask(GL_FALSE);
      if (gSettingsCache.show_scores)
				drawScore(p, d);
      if (gSettingsCache.show_ai_status)
				if(p->ai->active == AI_COMPUTER)
					drawAI(d);
    }
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
  }

  if (gSettingsCache.show_fps)
    drawFPS(gScreen);

	if(gSettingsCache.show_console)
		drawConsole(gScreen);

  /* printf("%d polys\n", polycount); */
}
