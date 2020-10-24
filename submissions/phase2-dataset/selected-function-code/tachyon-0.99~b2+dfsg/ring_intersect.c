  flt d;
  flt t,td;
  vector hit, pnt;
  
  d = -VDot(&(rng->ctr), &(rng->norm));
   
  t=-(d+VDot(&(rng->norm), &(ry->o)));
  td=VDot(&(rng->norm),&(ry->d)); 
  if (td != 0.0) {
    t= t / td;
    if (t>=0.0) {
      hit=Raypnt(ry, t);
      VSUB(hit, rng->ctr, pnt);
      VDOT(td, pnt, pnt);
      td=sqrt(td);
      if ((td > rng->inrad) && (td < rng->outrad)) 
        ry->add_intersection(t,(object *) rng, ry);
    }
  }
}
