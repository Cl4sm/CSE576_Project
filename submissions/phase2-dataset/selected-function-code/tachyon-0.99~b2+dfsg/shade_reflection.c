  ray specray;
  color col;
  vector R;

  /* Do recursion depth test immediately to early-exit ASAP */
  if (incident->depth <= 1) {
    /* if ray is truncated, return the background texture as its color */
    return incident->scene->bgtexfunc(incident);
  }
  specray.depth=incident->depth - 1;     /* go up a level in recursion depth */
  specray.transcnt=incident->transcnt;   /* maintain trans surface count */
 
  VAddS(-2.0 * (incident->d.x * shadevars->N.x + 
                incident->d.y * shadevars->N.y + 
                incident->d.z * shadevars->N.z), &shadevars->N, &incident->d, &R);

  specray.o=shadevars->hit; 
  specray.d=R;			         /* reflect incident ray about normal */
  specray.o=Raypnt(&specray, EPSILON);   /* avoid numerical precision bugs */
  specray.maxdist = FHUGE;               /* take any intersection */
  specray.opticdist = incident->opticdist;
  specray.add_intersection=incident->add_intersection; /* inherit ray type  */
  specray.flags = RT_RAY_REGULAR;        /* infinite ray, to start with */
  specray.serial = incident->serial + 1; /* next serial number */
  specray.mbox = incident->mbox; 
  specray.scene=incident->scene;         /* global scenedef info */
  specray.randval=incident->randval;     /* random number seed */
  specray.frng=incident->frng;           /* 32-bit FP RNG handle */

  /* inlined code from trace() to eliminate one level of recursion */
  intersect_objects(&specray);           /* trace specular reflection ray */
  col=specray.scene->shader(&specray);

  incident->serial = specray.serial;     /* update the serial number */
  incident->frng = specray.frng;         /* update AO RNG state      */

  ColorScale(&col, specular);

  return col;
}
