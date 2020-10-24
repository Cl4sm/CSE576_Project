  vector tvec, pvec, qvec;
  flt det, inv_det, t, u, v;

  /* begin calculating determinant - also used to calculate U parameter */
  CROSS(pvec, ry->d, trn->edge2);

  /* if determinant is near zero, ray lies in plane of triangle */
  det = DOT(trn->edge1, pvec);

#if 0           /* define TEST_CULL if culling is desired */
   if (det < EPSILON)
      return;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, ry->o, trn->v0);

   /* calculate U parameter and test bounds */
   u = DOT(tvec, pvec);
   if (u < 0.0 || u > det)
      return;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, trn->edge1);

   /* calculate V parameter and test bounds */
   v = DOT(ry->d, qvec);
   if (v < 0.0 || u + v > det)
      return;

   /* calculate t, scale parameters, ray intersects triangle */
   t = DOT(trn->edge2, qvec);
   inv_det = 1.0 / det;
   t *= inv_det;
   u *= inv_det;
   v *= inv_det;
#else                    /* the non-culling branch */
   if (det > -EPSILON && det < EPSILON)
     return;

   inv_det = 1.0 / det;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, ry->o, trn->v0);

   /* calculate U parameter and test bounds */
   u = DOT(tvec, pvec) * inv_det;
   if (u < 0.0 || u > 1.0)
     return;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, trn->edge1);

   /* calculate V parameter and test bounds */
   v = DOT(ry->d, qvec) * inv_det;
   if (v < 0.0 || u + v > 1.0)
     return;

   /* calculate t, ray intersects triangle */
   t = DOT(trn->edge2, qvec) * inv_det;
#endif

  ry->add_intersection(t,(object *) trn, ry);
}
