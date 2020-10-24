  flt a, tx1, tx2, ty1, ty2, tz1, tz2;
  flt tnear, tfar;

  tnear= -FHUGE;
  tfar= FHUGE;

  if (ry->d.x == 0.0) {
    if ((ry->o.x < g->min.x) || (ry->o.x > g->max.x)) return 0;
  } else {
    tx1 = (g->min.x - ry->o.x) / ry->d.x;
    tx2 = (g->max.x - ry->o.x) / ry->d.x;
    if (tx1 > tx2) { a=tx1; tx1=tx2; tx2=a; }
    if (tx1 > tnear) tnear=tx1;
    if (tx2 < tfar)   tfar=tx2;
  }
  if (tnear > tfar) return 0;
  if (tfar < 0.0) return 0;

  if (ry->d.y == 0.0) {
    if ((ry->o.y < g->min.y) || (ry->o.y > g->max.y)) return 0;
  } else {
    ty1 = (g->min.y - ry->o.y) / ry->d.y;
    ty2 = (g->max.y - ry->o.y) / ry->d.y;
    if (ty1 > ty2) { a=ty1; ty1=ty2; ty2=a; }
    if (ty1 > tnear) tnear=ty1;
    if (ty2 < tfar)   tfar=ty2;
  }
  if (tnear > tfar) return 0;
  if (tfar < 0.0) return 0;

  if (ry->d.z == 0.0) {
    if ((ry->o.z < g->min.z) || (ry->o.z > g->max.z)) return 0;
  } else {
    tz1 = (g->min.z - ry->o.z) / ry->d.z;
    tz2 = (g->max.z - ry->o.z) / ry->d.z;
    if (tz1 > tz2) { a=tz1; tz1=tz2; tz2=a; }
    if (tz1 > tnear) tnear=tz1;
    if (tz2 < tfar)   tfar=tz2;
  }
  if (tnear > tfar) return 0;
  if (tfar < 0.0) return 0;

  if (tnear < 0.0) {
    *hitnear = 0.0;
  } else {
    *hitnear = tnear;
  }

  *hitfar = tfar; 
  return 1;
}
