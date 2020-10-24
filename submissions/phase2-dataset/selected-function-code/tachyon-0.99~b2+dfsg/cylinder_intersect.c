static void cylinder_intersect(const cylinder * cyl, ray * ry) {
  vector rc, n, D, O;  
  flt t, s, tin, tout, ln, d; 

  rc.x = ry->o.x - cyl->ctr.x;
  rc.y = ry->o.y - cyl->ctr.y;
  rc.z = ry->o.z - cyl->ctr.z; 

  VCross(&ry->d, &cyl->axis, &n);

  ln=sqrt(n.x*n.x + n.y*n.y + n.z*n.z);    /* finish length calculation */

  if (ln == 0.0) {  /* ray is parallel to the cylinder.. */
    VDOT(d, rc, cyl->axis);         
    D.x = rc.x - d * cyl->axis.x; 
    D.y = rc.y - d * cyl->axis.y;
    D.z = rc.z - d * cyl->axis.z;
    VDOT(d, D, D);
    d = sqrt(d);
    tin = -FHUGE;
    tout = FHUGE;
    /* if (d <= cyl->rad) then ray is inside cylinder.. else outside */
  }

  n.x /= ln;
  n.y /= ln;
  n.z /= ln;

  VDOT(d, rc, n);
  d = fabs(d); 

  if (d <= cyl->rad) {  /* ray intersects cylinder.. */
    VCross(&rc, &cyl->axis, &O);
    VDOT(t, O, n);
    t = - t / ln;
    VCross(&n, &cyl->axis, &O); 

    ln = sqrt(O.x*O.x + O.y*O.y + O.z*O.z);
    O.x /= ln;
    O.y /= ln;
    O.z /= ln;

    VDOT(s, ry->d, O);
    s = fabs(sqrt(cyl->rad*cyl->rad - d*d) / s);
    tin = t - s;
    ry->add_intersection(tin, (object *) cyl, ry); 
    tout = t + s;
    ry->add_intersection(tout, (object *) cyl, ry);
  }
}
