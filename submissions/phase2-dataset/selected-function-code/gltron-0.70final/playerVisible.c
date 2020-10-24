int playerVisible(Player *eye, Player *target) {
  float v1[3];
  float v2[3];
  float tmp[3];
  float s;
  float d;
  int i;
  int lod_level;
	float x, y;

  vsub(eye->camera->target, eye->camera->cam, v1);
  normalize(v1);
	
	getPositionFromData(&x, &y, target->data);
	tmp[0] = x;
  tmp[1] = y;
  tmp[2] = 0;
	
  lod_level = (gSettingsCache.lod > MAX_LOD_LEVEL) ? 
    MAX_LOD_LEVEL : gSettingsCache.lod;

  /* calculate lod */
  vsub(eye->camera->cam, tmp, v2);
  d = length(v2);
  for(i = 0; i < LC_LOD && d >= lod_dist[lod_level][i]; i++);
  if(i >= LC_LOD)
    return -1;

  vsub(tmp, eye->camera->cam, v2);
  normalize(v2);
  s = scalarprod(v1, v2);
  /* maybe that's not exactly correct, but I didn't notice anything */
  d = cosf((gSettingsCache.fov / 2) * 2 * PI / 360.0);
  /*
    printf("v1: %.2f %.2f %.2f\nv2: %.2f %.2f %.2f\ns: %.2f d: %.2f\n\n",
    v1[0], v1[1], v1[2], v2[0], v2[1], v2[2],
    s, d);
  */
  if(s < d-(lightcycle[i]->BBox.fRadius*2))
    return -1;
  else
    return i;
}
