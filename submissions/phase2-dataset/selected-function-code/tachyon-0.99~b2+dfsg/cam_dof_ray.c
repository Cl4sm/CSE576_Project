  flt rdx, rdy, rdz, len;
  scenedef * scene=ry->scene;

  /* starting from the lower left corner of the image plane, we move the   */
  /* center of the pel we're calculating:                       */ 
  /* lowerleft + (rightvec * X_distance) + (upvec * Y_distance) */
  /* rdx/y/z are the ray directions (unnormalized)              */
  rdx = scene->camera.lowleft.x + 
                (x * scene->camera.iplaneright.x) + 
                (y * scene->camera.iplaneup.x) - ry->o.x;

  rdy = scene->camera.lowleft.y + 
                (x * scene->camera.iplaneright.y) + 
                (y * scene->camera.iplaneup.y) - ry->o.y;

  rdz = scene->camera.lowleft.z + 
                (x * scene->camera.iplaneright.z) + 
                (y * scene->camera.iplaneup.z) - ry->o.z;

  /* normalize the ray direction vector */
  len = 1.0 / sqrt(rdx*rdx + rdy*rdy + rdz*rdz);
  ry->d.x = rdx * len;
  ry->d.y = rdy * len;
  ry->d.z = rdz * len;

  /* initialize ray attributes for a primary ray */
  ry->maxdist = FHUGE;         /* unbounded ray */
  ry->opticdist = 0.0;         /* ray is just starting */

  /* camera only generates primary rays */
  ry->flags = RT_RAY_PRIMARY | RT_RAY_REGULAR;  

  ry->serial++;                /* increment the ray serial number */
  intersect_objects(ry);       /* trace the ray */
  return scene->shader(ry);    /* shade the hit point */
}
