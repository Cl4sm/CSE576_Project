void quadric_normal(const quadric * q, const vector * pnt, const ray * incident, vector * N) {
  flt invlen;

  N->x = (q->mat.a*(pnt->x - q->ctr.x) + 
	  q->mat.b*(pnt->y - q->ctr.y) + 
	  q->mat.c*(pnt->z - q->ctr.z) + q->mat.d);

  N->y = (q->mat.b*(pnt->x - q->ctr.x) + 
	  q->mat.e*(pnt->y - q->ctr.y) + 
	  q->mat.f*(pnt->z - q->ctr.z) + q->mat.g);

  N->z = (q->mat.c*(pnt->x - q->ctr.x) + 
	  q->mat.f*(pnt->y - q->ctr.y) + 
	  q->mat.h*(pnt->z - q->ctr.z) + q->mat.i);

  invlen = 1.0 / sqrt(N->x*N->x + N->y*N->y + N->z*N->z);
  N->x *= invlen;
  N->y *= invlen;
  N->z *= invlen;

  /* Flip surface normal to point toward the viewer if necessary */
  if (VDot(N, &(incident->d)) > 0.0)  {
    N->x=-N->x;
    N->y=-N->y;
    N->z=-N->z;
  }
}
