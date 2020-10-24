void drawCam(Player *p, PlayerVisual* pV) {
  int i;
  float up[3] = { 0, 0, 1 };
	Visual *d = & pV->display;
  
  glColor3f(0.0, 1.0, 0.0);
	
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  doPerspective(gSettingsCache.fov, d->vp_w / d->vp_h,
                gSettingsCache.znear, game2->rules.grid_size * 6.5f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  /* set positions for GL lights in world coordinates */
  glLightfv(GL_LIGHT1, GL_POSITION, p->camera->cam);

  doLookAt(p->camera->cam, p->camera->target, up);
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);

  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);

  /* skybox */
  if (gSettingsCache.show_skybox) {
    drawSkybox(game2->rules.grid_size);
  }

  /* fixme: clear z-buffer handling */
  /* glDepthMask(GL_TRUE); */
  
  /* floor */
  if (gSettingsCache.show_floor_texture) {
    drawFloorTextured(game2->rules.grid_size,
                      gScreen->textures[TEX_FLOOR]);
  } else {
    /* should this be an artpack setting? */
    float line_color[] = {1.0, 1.0, 1.0};
    
    drawFloorGrid(game2->rules.grid_size,
                  gSettingsCache.line_spacing,
                  line_color,
                  gSettingsCache.clear_color);
  }
  
  /* glDepthMask(GL_FALSE); */

  /* shadows on the floor: cycle, recognizer, trails */
  if (gSettingsCache.show_recognizer) {
    drawRecognizerShadow();
  }

  for(i = 0; i < game->players; i++) {
    int lod = playerVisible(p, game->player + i);
		if (lod >= 0) {
			int drawTurn = 1;
			if (! gSettingsCache.camType == CAM_TYPE_COCKPIT ||
	 			p != &game->player[i])
				drawTurn = 0;
			drawCycleShadow(gPlayerVisuals + i, game->player + i, lod, drawTurn);
		}
		if (game->player[i].data->trail_height > 0 )
			drawTrailShadow(game->player + i, gPlayerVisuals + i);
	}
	
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);

  if (gSettingsCache.show_recognizer &&
      p->data->speed != SPEED_GONE) {
    drawRecognizer();
  }

  if (gSettingsCache.show_wall == 1) {
    drawWalls();
  }

  drawPlayers(p, pV);

	setupLights(eWorld);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);

	{
		TrailMesh mesh;
		mesh.pVertices = (vec3*) malloc(1000 * sizeof(vec3));
		mesh.pNormals = (vec3*) malloc(1000 * sizeof(vec3));
		mesh.pColors = (unsigned char*) malloc(1000 * 4 * sizeof(float));
		mesh.pTexCoords = (vec2*) malloc(1000 * sizeof(vec2));
		mesh.pIndices = (unsigned short*) malloc(1000 * 2);

		for(i = 0; i < game->players; i++) {
			if (game->player[i].data->trail_height > 0 ) {
				int vOffset = 0;
				int iOffset = 0;
				mesh.iUsed = 0;
				trailGeometry(game->player + i, gPlayerVisuals + i,
											&mesh, &vOffset, &iOffset);
				bowGeometry(game->player + i, gPlayerVisuals + i,
										&mesh, &vOffset, &iOffset);
				trailStatesNormal(game->player + i, gScreen->textures[TEX_DECAL]);
				trailRender(&mesh);
				trailStatesRestore();
			}
		}
		free(mesh.pVertices);
		free(mesh.pNormals);
		free(mesh.pColors);
		free(mesh.pTexCoords);
		free(mesh.pIndices);
	}

  glDisable(GL_POLYGON_OFFSET_FILL);

  for(i = 0; i < game->players; i++)
    if (game->player[i].data->trail_height > 0 )
			drawTrailLines(game->player + i, gPlayerVisuals + i);

  /* transparent stuff */
  /* draw the glow around the other players: */
  if (gSettingsCache.show_glow == 1) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (i = 0; i < game->players; i++) {
      if (p != game->player + i && PLAYER_IS_ACTIVE(game->player + i)) {
	      drawGlow(p->camera, game->player + i, gPlayerVisuals + i,
								 d, TRAIL_HEIGHT * 4);
      }
      
    glDisable(GL_BLEND);
    }
  }
	/* 2d hack */
	if(gSettingsCache.map_ratio_w > 0)
	{
		Visual d2d;
		memcpy(&d2d, d, sizeof(Visual));
		d2d.vp_w *= gSettingsCache.map_ratio_w;
		d2d.vp_h *= gSettingsCache.map_ratio_h;

		d2d.vp_x += 20;
		d2d.vp_y += 20;
		
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		draw2D(&d2d);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}
}
