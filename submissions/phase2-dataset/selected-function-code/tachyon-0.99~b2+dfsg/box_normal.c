void box_normal(const box * bx, const vector * pnt, const ray * incident, vector * N) {
  vector a, b, c; 
  flt t;
 
  c.x=(bx->max.x + bx->min.x) / 2.0;
  c.y=(bx->max.y + bx->min.y) / 2.0;
  c.z=(bx->max.z + bx->min.z) / 2.0;
 
  VSub((vector *) pnt, &c, N);
  b=(*N);

  a.x=fabs(N->x);
  a.y=fabs(N->y);
  a.z=fabs(N->z);
 
  N->x=0.0;  N->y=0.0;  N->z=0.0;

  t=MYMAX(a.x, MYMAX(a.y, a.z));  

  if (t==a.x) N->x=b.x;  

  if (t==a.y) N->y=b.y; 

  if (t==a.z) N->z=b.z;

  VNorm(N);

  /* Flip surface normal to point toward the viewer if necessary */
  if (VDot(N, &(incident->d)) > 0.0)  {
    N->x=-N->x;
    N->y=-N->y;
    N->z=-N->z;
  }
}
