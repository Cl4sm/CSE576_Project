void rt_tri_ring(SceneHandle scene, void * tex, apivector ctr, apivector norm, flt a, flt b) {
  apivector x, y, z, tmp;
  double u, v, u2, v2;
  int j;
  apivector p1, p2, p3, p4;
  apivector n1, n2;

  z = norm;
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

  for (j=0; j<RINGFACETS; j++) {
     u = sin((6.28 * j) / (RINGFACETS - 1.0));
     v = cos((6.28 * j) / (RINGFACETS - 1.0));
    u2 = sin((6.28 * (j + 1.0)) / (RINGFACETS - 1.0));
    v2 = cos((6.28 * (j + 1.0)) / (RINGFACETS - 1.0));

    p1.x = p1.y = p1.z = 0.0;
    p4 = p3 = p2 = p1;

    VAddS(u, &x, &p1, &p1);
    VAddS(v, &y, &p1, &p1);
    n1 = p1;
    VNorm(&n1);
    VAddS(a, &n1, &ctr, &p1);
    VAddS(b, &n1, &ctr, &p3);

    VAddS(u2, &x, &p2, &p2);
    VAddS(v2, &y, &p2, &p2);
    n2 = p2;
    VNorm(&n2);
    VAddS(a, &n2, &ctr, &p2);
    VAddS(b, &n2, &ctr, &p4);

    rt_stri(scene, tex, p1, p2, p3, norm, norm, norm);
    rt_stri(scene, tex, p3, p2, p4, norm, norm, norm);

  }
} 
