  vector rc, n, O, hit, tmp2, ctmp4;
  flt t, s, tin, tout, ln, d, tmp, tmp3;
 
  rc.x = ry->o.x - cyl->ctr.x;  
  rc.y = ry->o.y - cyl->ctr.y;
  rc.z = ry->o.z - cyl->ctr.z;
 
  VCross(&ry->d, &cyl->axis, &n);
 
  ln=sqrt(n.x*n.x + n.y*n.y + n.z*n.z);    /* finish length calculation */
 
  if (ln == 0.0) {  /* ray is parallel to the cylinder.. */
    return;       /* in this case, we want to miss or go through the "hole" */
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

    RAYPNT(hit, (*ry), tin); 

    ctmp4=cyl->axis;
    VNorm(&ctmp4);

    tmp2.x = hit.x - cyl->ctr.x;   
    tmp2.y = hit.y - cyl->ctr.y;   
    tmp2.z = hit.z - cyl->ctr.z;   

    VDOT(tmp,  tmp2, ctmp4);
    VDOT(tmp3, cyl->axis, cyl->axis);

    if ((tmp > 0.0) && (tmp < sqrt(tmp3))) 
      ry->add_intersection(tin, (object *) cyl, ry);
    tout = t + s;

    RAYPNT(hit, (*ry), tout); 

    tmp2.x = hit.x - cyl->ctr.x;   
    tmp2.y = hit.y - cyl->ctr.y;   
    tmp2.z = hit.z - cyl->ctr.z;   

    VDOT(tmp,  tmp2, ctmp4); 
    VDOT(tmp3, cyl->axis, cyl->axis);

    if ((tmp > 0.0) && (tmp < sqrt(tmp3))) 
      ry->add_intersection(tout, (object *) cyl, ry);
  }
}
