directional_light * newdirectionallight(void * tex, vector dir) {
  directional_light * l;
  
  l=(directional_light *) malloc(sizeof(directional_light));
  memset(l, 0, sizeof(directional_light));
  l->methods = &light_methods;
  l->shade_diffuse = directional_light_shade_diffuse;

  l->tex=tex;
  l->tex->flags = RT_TEXTURE_SHADOWCAST | RT_TEXTURE_ISLIGHT;
  l->tex->diffuse=0.0;
  l->tex->specular=0.0;
  l->tex->opacity=1.0;

  l->dir.x=-dir.x; /* store negated light direction for high shading speed */
  l->dir.y=-dir.y;
  l->dir.z=-dir.z;

  return l;
}
