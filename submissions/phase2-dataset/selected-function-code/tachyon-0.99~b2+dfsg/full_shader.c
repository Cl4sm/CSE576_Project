color full_shader(ray * incident) {
  color col, diffuse, ambocccol, phongcol;
  shadedata shadevars;
  ray shadowray;
  flt inten;
  flt t = FHUGE;
  object const * obj;
  int numints;
  list * cur;

  numints=closest_intersection(&t, &obj, incident);  
		/* find the number of intersections */
                /* and return the closest one.      */

  if (numints < 1) {         
    /* if there weren't any object intersections then return the */
    /* background texture for the pixel color.                   */
    col=incident->scene->bgtexfunc(incident);

    /* Fog overrides the background color if we're using         */
    /* Tachyon radial fog, but not for OpenGL style fog.         */
    if (incident->scene->fog.type == RT_FOG_NORMAL &&
        incident->scene->fog.fog_fctn != NULL) {
      col = fog_color(incident, col, t);
    }

    return col;
  }

  RAYPNT(shadevars.hit, (*incident), t) /* find point of intersection from t */ 
  incident->opticdist += t;
  obj->methods->normal(obj, &shadevars.hit, incident, &shadevars.N);  /* find the surface normal */

  /* don't render transparent surfaces if we've reached the max count */
  if ((obj->tex->opacity < 1.0) && (incident->transcnt < 1)) {      
    /* spawn transmission rays / refraction */
    /* note: this will overwrite the old intersection list */
    return shade_transmission(incident, &shadevars, 1.0);
  }

  /* execute the object's texture function */
  col = obj->tex->texfunc(&shadevars.hit, obj->tex, incident); 

  if (obj->tex->flags & RT_TEXTURE_ISLIGHT) {  
                  /* if the current object is a light, then we  */
    return col;   /* will only use the object's base color      */
  }

  diffuse.r = 0.0; 
  diffuse.g = 0.0; 
  diffuse.b = 0.0; 
  ambocccol = diffuse;
  phongcol = diffuse;
  if ((obj->tex->diffuse > MINCONTRIB) || (obj->tex->phong > MINCONTRIB)) {  
    flt light_scale = incident->scene->light_scale;
    cur = incident->scene->lightlist;

    if (incident->scene->flags & RT_SHADE_CLIPPING) {
      shadowray.add_intersection = add_clipped_shadow_intersection;
    } else {
      shadowray.add_intersection = add_shadow_intersection;
    }
    shadowray.serial = incident->serial + 1; /* track ray serial number */
    shadowray.mbox = incident->mbox;
    shadowray.scene = incident->scene;

    while (cur != NULL) {              /* loop for light contributions */
      light * li=(light *) cur->item;  /* set li=to the current light  */
      inten = light_scale * li->shade_diffuse(li, &shadevars);

      /* add in diffuse lighting for this light if we're facing it */ 
      if (inten > MINCONTRIB) {            
        /* test for a shadow */
        shadowray.o   = shadevars.hit;
        shadowray.d   = shadevars.L;      
        shadowray.maxdist = shadevars.Llen;
        shadowray.flags = RT_RAY_SHADOW;
        shadowray.serial++;
        intersect_objects(&shadowray); /* trace the shadow ray */

        if (!shadow_intersection(&shadowray)) {
          /* If the light isn't occluded, then we modulate it by any */
          /* transparent surfaces the shadow ray encountered, and    */
          /* proceed with illumination calculations                  */
          inten *= shadowray.intstruct.shadowfilter;

          /* calculate diffuse lighting component */
          ColorAddS(&diffuse, &((standard_texture *)li->tex)->col, inten);

          /* phong type specular highlights */
          if (obj->tex->phong > MINCONTRIB) {
            flt phongval = light_scale * incident->scene->phongfunc(incident, &shadevars, obj->tex->phongexp); 
            if (obj->tex->phongtype == RT_PHONG_METAL) 
              ColorAddS(&phongcol, &col, phongval * obj->tex->phong);
            else
              ColorAddS(&phongcol, &((standard_texture *)li->tex)->col, phongval * obj->tex->phong);
          }
        }
      }  

      cur = cur->next;
    } 
    incident->serial = shadowray.serial; /* track ray serial number */

    /* add ambient occlusion lighting, if enabled */
    if (incident->scene->ambocc.numsamples > 0) { 
      ambocccol = shade_ambient_occlusion(incident, &shadevars);
    }
  }

  /* accumulate diffuse and ambient occlusion together */
  diffuse.r += ambocccol.r;
  diffuse.g += ambocccol.g;
  diffuse.b += ambocccol.b;

  if (obj->tex->outline > 0.0) {
    flt outlinefactor;
    flt edgefactor = VDot(&shadevars.N, &incident->d);
    edgefactor *= edgefactor;
    edgefactor = 1.0 - edgefactor;
    edgefactor = 1.0 - pow(edgefactor, (1.0 - obj->tex->outlinewidth) * 32.0);
    outlinefactor = (1.0-obj->tex->outline) + (edgefactor * obj->tex->outline);
    ColorScale(&diffuse, obj->tex->diffuse * outlinefactor);
  } else {
    ColorScale(&diffuse, obj->tex->diffuse);
  }

  /* do a product of the diffuse intensity with object color + ambient */
  col.r *= (diffuse.r + obj->tex->ambient);
  col.g *= (diffuse.g + obj->tex->ambient);
  col.b *= (diffuse.b + obj->tex->ambient);

  if (obj->tex->phong > MINCONTRIB) {
    ColorAccum(&col, &phongcol);
  }

  /* spawn reflection rays if necessary */
  /* note: this will overwrite the old intersection list */
  if (obj->tex->specular > MINCONTRIB) {    
    color specol;
    specol = shade_reflection(incident, &shadevars, obj->tex->specular);
    ColorAccum(&col, &specol);
  }

  /* spawn transmission rays / refraction */
  /* note: this will overwrite the old intersection list */
  if (obj->tex->opacity < (1.0 - MINCONTRIB)) {      
    color transcol;
    float alpha = obj->tex->opacity;

    /* Emulate Raster3D's angle-dependent surface opacity if enabled */    
    if ((incident->scene->transmode | obj->tex->transmode) & RT_TRANS_RASTER3D) {
      alpha = 1.0 + cos(3.1415926 * (1.0-alpha) * VDot(&shadevars.N, &incident->d));
      alpha = alpha*alpha * 0.25;
    }

    transcol = shade_transmission(incident, &shadevars, 1.0 - alpha);
    if (incident->scene->transmode & RT_TRANS_VMD) 
      ColorScale(&col, alpha);

    ColorAccum(&col, &transcol);
  }

  /* calculate fog effects */
  if (incident->scene->fog.fog_fctn != NULL) {
    col = fog_color(incident, col, t);
  }

  return col;    /* return the color of the shaded pixel... */
}
