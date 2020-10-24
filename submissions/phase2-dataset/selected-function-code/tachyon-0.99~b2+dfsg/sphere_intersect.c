  flt b, disc, t1, t2, temp;
  vector V;

  VSUB(spr->ctr, ry->o, V);
  VDOT(b, V, ry->d); 
  VDOT(temp, V, V);  

  disc=b*b + spr->rad*spr->rad - temp;

  if (disc<=0.0) return;
  disc=sqrt(disc);

  t2=b+disc;
  if (t2 <= SPEPSILON) 
    return;
  ry->add_intersection(t2, (object *) spr, ry);  

  t1=b-disc;
  if (t1 > SPEPSILON) 
    ry->add_intersection(t1, (object *) spr, ry);  
}
