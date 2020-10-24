color shade_transmission(ray * incident, const shadedata * shadevars, flt trans) {
  ray transray;
  color col;

  /* Do recursion depth test immediately to early-exit ASAP */
  if (incident->depth <= 1) {
    /* if ray is truncated, return the background texture as its color */
    return incident->scene->bgtexfunc(incident);
  }
  transray.o=shadevars->hit; 
  transray.d=incident->d;                 /* ray continues on incident path */
  transray.o=Raypnt(&transray, EPSILON);  /* avoid numerical precision bugs */
  transray.maxdist = FHUGE;               /* take any intersection */
  transray.opticdist = incident->opticdist;
  transray.add_intersection=incident->add_intersection; /* inherit ray type  */
  transray.depth=incident->depth - 1;     /* track recursion depth           */
  transray.transcnt=incident->transcnt-1; /* maintain trans surface count    */
  transray.flags = RT_RAY_REGULAR;        /* infinite ray, to start with */
  transray.serial = incident->serial + 1; /* update serial number */
  transray.mbox = incident->mbox;
  transray.scene=incident->scene;         /* global scenedef info */
  transray.randval=incident->randval;     /* random number seed */
  transray.frng=incident->frng;           /* 32-bit FP RNG handle */

  /* inlined code from trace() to eliminate one level of recursion */
  intersect_objects(&transray);           /* trace transmission ray */
  col=transray.scene->shader(&transray);

  incident->serial = transray.serial;     /* update the serial number */
  incident->frng = transray.frng;         /* update AO RNG state      */

  ColorScale(&col, trans);

  return col;
}
