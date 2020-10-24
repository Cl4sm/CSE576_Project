void rt_tri_fcylinder(SceneHandle scene, void * tex, apivector ctr, apivector axis, flt rad) {
  apivector x, y, z, tmp;
  double u, v, u2, v2;
  int j;
  apivector p1, p2, p3, p4;
  apivector n1, n2;

  z = axis;
  VNorm(&z);
  tmp.x = z.y - 2.1111111;
  tmp.y = -z.z + 3.14159267;
  tmp.z = z.x - 3.915292342341;
  VNorm(&z);
  VNorm(&tmp);
  VCross(&z, &tmp, &x);
  VNorm(&x);
  VCross(&x, &z, &y);
  VNorm(&y);

  for (j=0; j<CYLFACETS; j++) {
     u = rad * sin((6.28 * j) / (CYLFACETS - 1.0));
     v = rad * cos((6.28 * j) / (CYLFACETS - 1.0));
    u2 = rad * sin((6.28 * (j + 1.0)) / (CYLFACETS - 1.0));
    v2 = rad * cos((6.28 * (j + 1.0)) / (CYLFACETS - 1.0));

    p1.x = p1.y = p1.z = 0.0;
    p4 = p3 = p2 = p1;

    VAddS(u, &x, &p1, &p1);
    VAddS(v, &y, &p1, &p1);
    n1 = p1;
    VNorm(&n1);
    VAddS(1.0, &ctr, &p1, &p1);
  

    VAddS(u2, &x, &p2, &p2);
    VAddS(v2, &y, &p2, &p2);
    n2 = p2;
    VNorm(&n2);
    VAddS(1.0, &ctr, &p2, &p2);

    VAddS(1.0, &axis, &p1, &p3);
    VAddS(1.0, &axis, &p2, &p4);

    rt_stri(scene, tex, p1, p2, p3, n1, n2, n1);
    rt_stri(scene, tex, p3, p2, p4, n1, n2, n2);
  }
}
