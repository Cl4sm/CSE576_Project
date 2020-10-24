color cam_aa_dof_ray(ray * ry, flt x, flt y) {
  color col, avcol;
  int alias; 
  scenedef * scene=ry->scene;
  float scale;
  flt dx, dy;

  col=cam_dof_ray(ry, x, y);   /* generate ray */

  /* perform antialiasing if enabled.                           */
  /* samples are run through a very simple box filter averaging */
  /* each of the sample pixel colors to produce a final result  */
  /* No special weighting is done based on the jitter values in */
  /* the circle of confusion nor for the jitter within the      */
  /* pixel in the image plane.                                  */ 
  for (alias=1; alias <= scene->antialiasing; alias++) {
    float jxy[2];
    jitter_offset2f(&ry->randval, jxy);
    dx = jxy[0] * ry->scene->camera.aperture * ry->scene->hres; 
    dy = jxy[1] * ry->scene->camera.aperture * ry->scene->vres; 

    /* perturb the eye center by the random aperture offset */
    ry->o.x = ry->scene->camera.center.x + 
              dx * ry->scene->camera.iplaneright.x +
              dy * ry->scene->camera.iplaneup.x;
    ry->o.y = ry->scene->camera.center.y + 
              dx * ry->scene->camera.iplaneright.y +
              dy * ry->scene->camera.iplaneup.y;
    ry->o.z = ry->scene->camera.center.z + 
              dx * ry->scene->camera.iplaneright.z +
              dy * ry->scene->camera.iplaneup.z;

    /* shoot the ray, jittering the pixel position in the image plane */
    jitter_offset2f(&ry->randval, jxy);
    avcol=cam_dof_ray(ry, x + jxy[0], y + jxy[1]);

    col.r += avcol.r;       /* accumulate antialiasing samples */
    col.g += avcol.g;
    col.b += avcol.b;
  }

  /* average sample colors, back to range 0.0 - 1.0 */ 
  scale = 1.0f / (scene->antialiasing + 1.0f); 
  col.r *= scale;
  col.g *= scale;
  col.b *= scale;

  return col;
}
