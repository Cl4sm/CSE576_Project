void camray_init(scenedef *scene, ray *primary, unsigned long serial, 
                 unsigned long * mbox, unsigned int randval) {
  /* setup the right function pointer depending on what features are in use */
  if (scene->flags & RT_SHADE_CLIPPING) {
    primary->add_intersection = add_clipped_intersection;
  } else {
    primary->add_intersection = add_regular_intersection;
  }

  primary->serial = serial;
  primary->mbox = mbox;
  primary->scene = scene;
  primary->depth = scene->raydepth;      /* set to max ray depth      */
  primary->transcnt = scene->transcount; /* set to max trans surf cnt */
  primary->randval = randval;            /* random number seed */
  rng_frand_init(&primary->frng);        /* seed 32-bit FP RNG */

  /* orthographic ray direction is always coaxial with view direction */
  primary->d = scene->camera.viewvec; 

  /* for perspective rendering without depth of field */
  primary->o = scene->camera.center;
}
