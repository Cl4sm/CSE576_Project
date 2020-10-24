  color col, avcol;
  int alias; 
  scenedef * scene=ry->scene;
  float scale;

  col=cam_orthographic_ray(ry, x, y);   /* generate ray */

  /* perform antialiasing if enabled.                           */
  /* samples are run through a very simple box filter averaging */
  /* each of the sample pixel colors to produce a final result  */
  for (alias=1; alias <= scene->antialiasing; alias++) {
    float jxy[2];
    jitter_offset2f(&ry->randval, jxy);
    avcol=cam_orthographic_ray(ry, x + jxy[0], y + jxy[1]);

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
