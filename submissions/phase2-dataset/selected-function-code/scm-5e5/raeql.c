static int raeql(ra0, as_equal, ra1)
     SCM ra0, as_equal, ra1;
{
  SCM v0 = ra0, v1 = ra1;
  array_dim dim0, dim1;
  array_dim *s0 = &dim0, *s1 = &dim1;
  sizet bas0 = 0, bas1 = 0;
  int k, unroll = 1, ndim = 1;
  if (ARRAYP(ra0)) {
    ndim = ARRAY_NDIM(ra0);
    s0 = ARRAY_DIMS(ra0);
    bas0 = ARRAY_BASE(ra0);
    v0 = ARRAY_V(ra0);
  }
  else {
    s0->inc = 1; s0->lbnd = 0; s0->ubnd = LENGTH(v0) - 1;
  }
  if (ARRAYP(ra1)) {
    if (ndim != ARRAY_NDIM(ra1)) return 0;
    s1 = ARRAY_DIMS(ra1);
    bas1 = ARRAY_BASE(ra1);
    v1 = ARRAY_V(ra1);
  }
  else {
    if (1 != ndim) return BOOL_F;
    s1->inc = 1; s1->lbnd = 0; s1->ubnd = LENGTH(v1) - 1;
  }
  if (TYP7(v0) != TYP7(v1)) return 0;
  unroll = (bas0==bas1);
  for (k = ndim; k--;) {
    if (s0[k].lbnd != s1[k].lbnd || s0[k].ubnd != s1[k].ubnd) return 0;
    if (unroll) unroll = (s0[k].inc==s1[k].inc);
  }
  if (unroll && v0==v1) return BOOL_T;
  return ramapc(raeql_1, as_equal, ra0, cons(ra1, EOL), "");
}
