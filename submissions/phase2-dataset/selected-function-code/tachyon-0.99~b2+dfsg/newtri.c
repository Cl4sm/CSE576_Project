  tri * t;
  vector edge1, edge2, edge3;

  VSub(&v1, &v0, &edge1);
  VSub(&v2, &v0, &edge2);
  VSub(&v2, &v1, &edge3);

  /* check to see if this will be a degenerate triangle before creation */
  if ((VLength(&edge1) >= EPSILON) && 
      (VLength(&edge2) >= EPSILON) && 
      (VLength(&edge3) >= EPSILON)) {

    t=(tri *) malloc(sizeof(tri));

    t->nextobj = NULL;
    t->methods = &tri_methods;

    t->tex = tex;
    t->v0 = v0;
    t->edge1 = edge1;
    t->edge2 = edge2;
 
    return (object *) t;
  }
  
  return NULL; /* was a degenerate triangle */
}
