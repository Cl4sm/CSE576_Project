void * newscalarvol(void * voidtex, vector min, vector max, 
                    int xs, int ys, int zs, const char * fname, 
                    scalarvol * invol) {
  standard_texture * tx, * tex;
  scalarvol * vol;

  tex=(standard_texture *) voidtex;
  tex->flags = RT_TEXTURE_NOFLAGS; /* doesn't cast a shadow */

  tx=malloc(sizeof(standard_texture));

  /* is the volume data already loaded? */
  if (invol==NULL) {
    vol=malloc(sizeof(scalarvol));
    vol->loaded=0;
    vol->data=NULL;
  } else {
    vol=invol;
  }

  vol->opacity=tex->opacity;
  vol->xres=xs;
  vol->yres=ys;
  vol->zres=zs;
  strcpy(vol->name, fname);

  tx->ctr.x = 0.0;
  tx->ctr.y = 0.0;
  tx->ctr.z = 0.0;
  tx->rot   = tx->ctr;
  tx->scale = tx->ctr;
  tx->uaxs  = tx->ctr;
  tx->vaxs  = tx->ctr;

  tx->flags = RT_TEXTURE_NOFLAGS;

  tx->col = tex->col;
  tx->ambient   = 1.0;
  tx->diffuse   = 0.0;
  tx->phong     = 0.0;
  tx->phongexp  = 0.0;
  tx->phongtype = 0;
  tx->specular  = 0.0;
  tx->opacity   = 1.0;
  tx->outline   = 0.0;
  tx->outlinewidth = 0.0;
  tx->img = vol;
  tx->texfunc = (color(*)(const void *, const void *, void *))(scalar_volume_texture);

  tx->obj = (void *) newbox(tx, min, max); /* XXX hack!! */

  /* Force load of volume data so that we don't have to do mutex locks */
  /* inside the rendering threads                                      */
  if (!vol->loaded) {
    LoadVol(vol);
  }

  /* check if loading succeeded */
  if (!vol->loaded) {
    tx->texfunc = (color(*)(const void *, const void *, void *))(constant_texture);
    tx->img = NULL;
    free(vol);
  }

  return (void *) tx->obj;
}
