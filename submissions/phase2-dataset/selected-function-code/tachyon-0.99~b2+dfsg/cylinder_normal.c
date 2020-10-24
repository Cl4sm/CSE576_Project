  vector a, b;
  flt t, invlen, invlen2;

  a.x = pnt->x - cyl->ctr.x;
  a.y = pnt->y - cyl->ctr.y;
  a.z = pnt->z - cyl->ctr.z;

  b=cyl->axis;

  invlen = 1.0 / sqrt(b.x*b.x + b.y*b.y + b.z*b.z);
  b.x *= invlen;
  b.y *= invlen;
  b.z *= invlen;
 
  VDOT(t, a, b);

  N->x = pnt->x - (b.x * t + cyl->ctr.x); 
  N->y = pnt->y - (b.y * t + cyl->ctr.y);
  N->z = pnt->z - (b.z * t + cyl->ctr.z);

  invlen2 = 1.0 / sqrt(N->x*N->x + N->y*N->y + N->z*N->z);
  N->x *= invlen2;
  N->y *= invlen2;
  N->z *= invlen2;

  /* Flip surface normal to point toward the viewer if necessary */
  if (VDot(N, &(incident->d)) > 0.0)  {
    N->x=-N->x;
    N->y=-N->y;
    N->z=-N->z;
  }
}
