  ray ambray;
  color ambcol;
  int i;
  flt ndotambl;
  flt inten = 0.0;

  /* The integrated hemisphere for an unweighted non-importance-sampled  */
  /* ambient occlusion case has a maximum sum (when uniformly sampled)   */
  /* of 0.5 relative to an similar direct illumination case oriented     */
  /* exactly with the surface normal.  So, we prescale the normalization */
  /* for ambient occlusion by a factor of 2.0.  This will have to change */
  /* when importance sampling is implemented.  If a small number of      */
  /* occlusion samples are taken, and they coincidentally end up facing  */
  /* with the surface normal, we could exceed the expected normalization */
  /* factor, but the results will be correctly clamped by the rest of    */
  /* shading code, so we don't worry about it here.                      */
  flt lightscale = 2.0 / incident->scene->ambocc.numsamples;

  ambray.o=shadevars->hit;
  ambray.d=shadevars->N;
  ambray.o=Raypnt(&ambray, EPSILON);    /* avoid numerical precision bugs */
  ambray.serial = incident->serial + 1; /* next serial number */
  ambray.randval=incident->randval;     /* random number seed */
  ambray.frng=incident->frng;           /* 32-bit FP RNG handle */
  if (incident->scene->flags & RT_SHADE_CLIPPING) {
    ambray.add_intersection = add_clipped_shadow_intersection;
  } else {
    ambray.add_intersection = add_shadow_intersection;
  }
  ambray.mbox = incident->mbox; 
  ambray.scene=incident->scene;         /* global scenedef info */

  for (i=0; i<incident->scene->ambocc.numsamples; i++) {
    float dir[3];
    ambray.maxdist = FHUGE;         /* take any intersection */
    ambray.flags = RT_RAY_SHADOW;   /* shadow ray */
    ambray.serial++;

    /* generate a randomly oriented ray */
    jitter_sphere3f(&ambray.frng, dir);
    ambray.d.x = dir[0];
    ambray.d.y = dir[1];
    ambray.d.z = dir[2];

    /* flip the ray so it's in the same hemisphere as the surface normal */
    ndotambl = VDot(&ambray.d, &shadevars->N);
    if (ndotambl < 0) {
      ndotambl   = -ndotambl;
      ambray.d.x = -ambray.d.x;
      ambray.d.y = -ambray.d.y;
      ambray.d.z = -ambray.d.z;
    }

    intersect_objects(&ambray); /* trace the shadow ray */

    /* if no objects were hit, add an ambient contribution */
    if (!shadow_intersection(&ambray)) {
      /* If the light isn't occluded, then we modulate it by any */
      /* transparent surfaces the shadow ray encountered, and    */
      /* proceed with illumination calculations                  */
      ndotambl *= ambray.intstruct.shadowfilter;

      inten += ndotambl;
    }
  }
  ambcol.r = lightscale * inten * incident->scene->ambocc.col.r;
  ambcol.g = lightscale * inten * incident->scene->ambocc.col.g;
  ambcol.b = lightscale * inten * incident->scene->ambocc.col.b;

  incident->serial = ambray.serial + 1;     /* update the serial number */
  incident->frng = ambray.frng;             /* update AO RNG state      */

  return ambcol; 
}
