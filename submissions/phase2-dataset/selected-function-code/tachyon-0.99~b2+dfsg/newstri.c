                           vector n0, vector n1, vector n2) {
  stri * t;
  vector edge1, edge2, edge3;

  VSub(&v1, &v0, &edge1);
  VSub(&v2, &v0, &edge2);
  VSub(&v2, &v1, &edge3);

  /* check to see if this will be a degenerate triangle before creation */
  if ((VLength(&edge1) >= EPSILON) && 
      (VLength(&edge2) >= EPSILON) &&
      (VLength(&edge3) >= EPSILON)) {

    t=(stri *) malloc(sizeof(stri));

    t->nextobj = NULL;
    t->methods = &stri_methods;
 
    t->tex = tex;
    t->v0 = v0;
    t->edge1 = edge1;
    t->edge2 = edge2;
    t->n0 = n0;
    t->n1 = n1;
    t->n2 = n2;

    return (object *) t;
  }

  return NULL; /* was a degenerate triangle */
}
