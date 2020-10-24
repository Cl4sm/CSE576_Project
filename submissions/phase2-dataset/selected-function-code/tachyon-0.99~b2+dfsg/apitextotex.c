  standard_texture * tex = (standard_texture *) tx;
  tex->img = NULL;
 
  switch(apitex->texturefunc) {
    case RT_TEXTURE_3D_CHECKER: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(checker_texture);
      break;

    case RT_TEXTURE_GRIT: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(grit_texture);
      break;

    case RT_TEXTURE_MARBLE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(marble_texture);
      break;

    case RT_TEXTURE_WOOD: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(wood_texture);
      break;

    case RT_TEXTURE_GRADIENT: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(gnoise_texture);
      break;
	
    case RT_TEXTURE_CYLINDRICAL_CHECKER: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(cyl_checker_texture);
      break;

    case RT_TEXTURE_CYLINDRICAL_IMAGE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(image_cyl_texture);
      tex->img=LoadMIPMap(apitex->imap, 0);
      break;

    case RT_TEXTURE_SPHERICAL_IMAGE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(image_sphere_texture);
      tex->img=LoadMIPMap(apitex->imap, 0);
      break;

    case RT_TEXTURE_PLANAR_IMAGE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(image_plane_texture);
      tex->img=LoadMIPMap(apitex->imap, 0);
      break;

    case RT_TEXTURE_VOLUME_IMAGE: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(image_volume_texture);
      tex->img=LoadMIPMap(apitex->imap, 0);
      break;

    case RT_TEXTURE_CONSTANT: 
    default: 
      tex->texfunc=(color(*)(const void *, const void *, void *))(constant_texture);
      break;
  }

       tex->ctr = apitex->ctr;
       tex->rot = apitex->rot;
     tex->scale = apitex->scale;
      tex->uaxs = apitex->uaxs;
      tex->vaxs = apitex->vaxs;
      tex->waxs = apitex->waxs;
   tex->ambient = apitex->ambient;
   tex->diffuse = apitex->diffuse;
  tex->specular = apitex->specular;
   tex->opacity = apitex->opacity;
       tex->col = apitex->col; 

  /* initialize texture flags */
  tex->flags = RT_TEXTURE_NOFLAGS;

  /* anything less than an entirely opaque object will modulate */
  /* the light intensity rather than completly occluding it     */
  if (apitex->opacity >= 0.99999)
    tex->flags = RT_TEXTURE_SHADOWCAST;

  tex->phong = 0.0;
  tex->phongexp = 0.0;
  tex->phongtype = 0;

  tex->transmode = RT_TRANS_ORIG;

  tex->outline = 0.0;
  tex->outlinewidth = 0.0;
}
