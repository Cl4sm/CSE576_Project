  scenedef * scene=ry->scene;

  /* starting from the lower left corner of the image plane, we move the   */
  /* center of the pel we're calculating:                       */ 
  /* lowerleft + (rightvec * X_distance) + (upvec * Y_distance) */
  ry->o.x = scene->camera.lowleft.x + 
                (x * scene->camera.iplaneright.x) + 
                (y * scene->camera.iplaneup.x);

  ry->o.y = scene->camera.lowleft.y + 
                (x * scene->camera.iplaneright.y) + 
                (y * scene->camera.iplaneup.y);

  ry->o.z = scene->camera.lowleft.z + 
                (x * scene->camera.iplaneright.z) + 
                (y * scene->camera.iplaneup.z);

  /* initialize ray attributes for a primary ray */
  ry->maxdist = FHUGE;         /* unbounded ray */
  ry->opticdist = 0.0;         /* ray is just starting */

  /* camera only generates primary rays */
  ry->flags = RT_RAY_PRIMARY | RT_RAY_REGULAR;  

  ry->serial++;                /* increment the ray serial number */
  intersect_objects(ry);       /* trace the ray */
  return scene->shader(ry);    /* shade the hit point */
}
