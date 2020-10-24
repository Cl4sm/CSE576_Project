extvol * newextvol(void * voidtex, vector min, vector max, 
                   int samples, flt (* evaluator)(flt, flt, flt)) { 
  extvol * xvol;
  standard_texture * tex, * xvoltex;
  
  tex = (standard_texture *) voidtex;

  xvol = (extvol *) malloc(sizeof(extvol));
  memset(xvol, 0, sizeof(extvol));

  xvol->methods = &extvol_methods;

  xvol->min=min;
  xvol->max=max;
  xvol->evaluator = evaluator;
  xvol->ambient = tex->ambient;
  xvol->diffuse = tex->diffuse;
  xvol->opacity = tex->opacity;  
  xvol->samples = samples;

  xvoltex = malloc(sizeof(standard_texture));
  memset(xvoltex, 0, sizeof(standard_texture));

  xvoltex->ctr.x = 0.0;
  xvoltex->ctr.y = 0.0;
  xvoltex->ctr.z = 0.0;
  xvoltex->rot = xvoltex->ctr;
  xvoltex->scale = xvoltex->ctr;
  xvoltex->uaxs = xvoltex->ctr;
  xvoltex->vaxs = xvoltex->ctr;
  xvoltex->flags = RT_TEXTURE_NOFLAGS;

  xvoltex->col=tex->col;
  xvoltex->ambient=1.0;
  xvoltex->diffuse=0.0;
  xvoltex->specular=0.0;
  xvoltex->opacity=1.0;
  xvoltex->img=NULL;
  xvoltex->texfunc=(color(*)(const void *, const void *, void *))(ext_volume_texture);
  xvoltex->obj = (void *) xvol; /* XXX hack! */

  xvol->tex = (texture *) xvoltex;

  return xvol;
}
