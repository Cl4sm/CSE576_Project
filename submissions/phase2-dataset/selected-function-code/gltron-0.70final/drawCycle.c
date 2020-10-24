void drawCycle(Player *p, PlayerVisual *pV, int lod, int drawTurn) {
  Mesh *cycle = lightcycle[lod];

  unsigned int spoke_time = game2->time.current - pV->spoke_time;
	int turn_time = game2->time.current - p->data->turn_time;

  if(turn_time < TURN_LENGTH && !drawTurn)
    return;
  
  glPushMatrix();
	{
		float x, y;
		getPositionFromData(&x, &y, p->data);
		glTranslatef(x, y, 0.0);
	}

  if (pV->exp_radius == 0 && gSettingsCache.turn_cycle == 0) {
    glRotatef(dirangles[p->data->dir], 0.0, 0.0, 1.0);
  }

  if (gSettingsCache.turn_cycle) { 
    doCycleTurnRotation(pV, p);
  }

	setupLights(eCycles);
	
  SetMaterialColor(cycle, "Hull", eDiffuse, pV->pColorDiffuse); 
  SetMaterialColor(cycle, "Hull", eSpecular, pV->pColorSpecular); 

  if (gSettingsCache.light_cycles) {
    glEnable(GL_LIGHTING);
  }

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  if (pV->exp_radius == 0) {
    glEnable(GL_NORMALIZE);

    glTranslatef(0, 0, cycle->BBox.vSize.v[2] / 2);

    /* draw spoke animation */
    if (spoke_time > 140 - (p->data->speed * 10) 
        && game->pauseflag == PAUSE_GAME_RUNNING) {
      if (pV->spoke_state == 1) {
        pV->spoke_state = 0;
        SetMaterialColor(cycle, "Spoke", eSpecular, SpokeColor);
        SetMaterialColor(cycle, "Spoke", eAmbient, SpokeColor);
      } else {
        pV->spoke_state = 1;
        SetMaterialColor(cycle, "Spoke", eSpecular, NoSpokeColor);
        SetMaterialColor(cycle, "Spoke", eAmbient, NoSpokeColor);
      }
      pV->spoke_time = game2->time.current;
    }
    
    glEnable(GL_CULL_FACE);
    drawModel(cycle, TRI_MESH);
    glDisable(GL_CULL_FACE);

  } else if(pV->exp_radius < EXP_RADIUS_MAX) {
   
    glEnable(GL_BLEND);

    if (gSettingsCache.show_impact) {
      drawImpact(pV);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glTranslatef(0, 0, cycle->BBox.vSize.v[2] / 2);

    drawModelExplosion(cycle, pV->exp_radius);
  }
  glDisable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glPopMatrix();
}
