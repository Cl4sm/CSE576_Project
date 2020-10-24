  flt a, tx1, tx2, ty1, ty2, tz1, tz2;
  flt tnear, tfar;

  tnear= -FHUGE;
  tfar= FHUGE;

  if (ry->d.x == 0.0) {
    if ((ry->o.x < bx->min.x) || (ry->o.x > bx->max.x)) return;
  }
  else {
    tx1 = (bx->min.x - ry->o.x) / ry->d.x;
    tx2 = (bx->max.x - ry->o.x) / ry->d.x;
    if (tx1 > tx2) { a=tx1; tx1=tx2; tx2=a; } 
    if (tx1 > tnear) tnear=tx1;   
    if (tx2 < tfar)   tfar=tx2;   
  } 
  if (tnear > tfar) return; 
  if (tfar < 0.0) return;
  
  if (ry->d.y == 0.0) { 
    if ((ry->o.y < bx->min.y) || (ry->o.y > bx->max.y)) return;
  }
  else {
    ty1 = (bx->min.y - ry->o.y) / ry->d.y;
    ty2 = (bx->max.y - ry->o.y) / ry->d.y;
    if (ty1 > ty2) { a=ty1; ty1=ty2; ty2=a; } 
    if (ty1 > tnear) tnear=ty1;   
    if (ty2 < tfar)   tfar=ty2;   
  }
  if (tnear > tfar) return; 
  if (tfar < 0.0) return;
 
  if (ry->d.z == 0.0) { 
    if ((ry->o.z < bx->min.z) || (ry->o.z > bx->max.z)) return;
  }
  else {
    tz1 = (bx->min.z - ry->o.z) / ry->d.z;
    tz2 = (bx->max.z - ry->o.z) / ry->d.z;
    if (tz1 > tz2) { a=tz1; tz1=tz2; tz2=a; } 
    if (tz1 > tnear) tnear=tz1;   
    if (tz2 < tfar)   tfar=tz2;   
  }
  if (tnear > tfar) return; 
  if (tfar < 0.0) return;

  ry->add_intersection(tnear, (const object *) bx, ry);
  ry->add_intersection(tfar, (object *) bx, ry);
}
