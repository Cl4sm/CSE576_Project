void resetVideoData(void) {
  /* for each player */

  int i;
  for(i = 0; i < game->players; i++) {
		PlayerVisual *pV = gPlayerVisuals + i;
    {
      char name[32];
      sprintf(name, "model_diffuse_%d", i);
			scripting_GetGlobal(name, NULL);
      scripting_GetFloatArrayResult(pV->pColorDiffuse, 4);
      sprintf(name, "model_specular_%d", i);
			scripting_GetGlobal(name, NULL);
      scripting_GetFloatArrayResult(pV->pColorSpecular, 4);
      sprintf(name, "trail_diffuse_%d", i);
			scripting_GetGlobal(name, NULL);
      scripting_GetFloatArrayResult(pV->pColorAlpha, 4);
    }
		if(game->player[i].ai->active != AI_NONE) {
			pV->impact_radius = 0.0;
			pV->exp_radius = 0;
		} else {
			pV->exp_radius = EXP_RADIUS_MAX;
		}

  }
}
