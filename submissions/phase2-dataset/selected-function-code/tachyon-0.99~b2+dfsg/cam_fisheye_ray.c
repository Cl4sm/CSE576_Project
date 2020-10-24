  flt ax, ay;
  scenedef * scene=ry->scene;

  ax = scene->camera.left   + x * scene->camera.psx;
  ay = scene->camera.bottom + y * scene->camera.psy;

  ry->d.x = cos(ay) * (cos(ax) * scene->camera.viewvec.x + 
                       sin(ax) * scene->camera.rightvec.x) +
            sin(ay) * scene->camera.upvec.x;

  ry->d.y = cos(ay) * (cos(ax) * scene->camera.viewvec.y +
                       sin(ax) * scene->camera.rightvec.y) +
            sin(ay) * scene->camera.upvec.y;

  ry->d.z = cos(ay) * (cos(ax) * scene->camera.viewvec.z +
                       sin(ax) * scene->camera.rightvec.z) +
            sin(ay) * scene->camera.upvec.z;
        
  /* initialize ray attributes for a primary ray */
  ry->maxdist = FHUGE;         /* unbounded ray */
  ry->opticdist = 0.0;         /* ray is just starting */

  /* camera only generates primary rays */
  ry->flags = RT_RAY_PRIMARY | RT_RAY_REGULAR;  

  ry->serial++;                /* increment the ray serial number */
  intersect_objects(ry);       /* trace the ray */
  return scene->shader(ry);    /* shade the hit point */
}
