  point_light * l;
  
  l=(point_light *) malloc(sizeof(point_light));
  memset(l, 0, sizeof(point_light));
  l->methods = &light_methods;
  l->shade_diffuse = simple_point_light_shade_diffuse;

  l->tex=tex;
  l->tex->flags = RT_TEXTURE_SHADOWCAST | RT_TEXTURE_ISLIGHT;
  l->tex->diffuse=0.0;
  l->tex->specular=0.0;
  l->tex->opacity=1.0;

  l->ctr=ctr;
  l->rad=rad;

  l->attenuationfunc = light_no_attenuation;
  l->Kc = 1.0;
  l->Kl = 0.0;
  l->Kq = 0.0;

  l->spotfunc = light_no_falloff;
  l->spotdir.x = 0.0;
  l->spotdir.y = 0.0;
  l->spotdir.z = 1.0;
  l->fallstart = 0.3;
  l->fallend   = 0.7;

  return l;
}
