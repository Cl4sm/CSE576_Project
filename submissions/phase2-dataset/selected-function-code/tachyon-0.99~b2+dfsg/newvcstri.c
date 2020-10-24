object * newvcstri(void * voidtex, vector v0, vector v1, vector v2,
                   vector n0, vector n1, vector n2,
                   color c0, color c1, color c2) {
  vcstri * t;
  vector edge1, edge2, edge3;
  vcstri_texture * tex = (vcstri_texture *) voidtex; 

  VSub(&v1, &v0, &edge1);
  VSub(&v2, &v0, &edge2);
  VSub(&v2, &v1, &edge3);

  /* check to see if this will be a degenerate triangle before creation */
  if ((VLength(&edge1) >= EPSILON) && 
      (VLength(&edge2) >= EPSILON) &&
      (VLength(&edge3) >= EPSILON)) {

    t=(vcstri *) malloc(sizeof(vcstri));

    t->nextobj = NULL;
    t->methods = &stri_methods;
 
    t->v0 = v0;
    t->edge1 = edge1;
    t->edge2 = edge2;
    t->n0 = n0;
    t->n1 = n1;
    t->n2 = n2;

    tex->c0 = c0;
    tex->c1 = c1;
    tex->c2 = c2;
    tex->obj = t; /* XXX hack to let the texture function get c0c1c2 data */
    tex->texfunc = (color(*)(const void *, const void *, void *))(vcstri_color);
    t->tex = (texture *) tex;

    return (object *) t;
  }

  return NULL; /* was a degenerate triangle */
}
