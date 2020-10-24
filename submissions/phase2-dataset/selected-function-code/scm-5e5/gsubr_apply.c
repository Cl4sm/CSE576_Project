SCM gsubr_apply(args)
     SCM args;
{
  SCM self = CAR(args);
  SCM (*fcn)() = SUBRF(GSUBR_PROC(self));
  int typ = INUM(GSUBR_TYPE(self));
  int i, n = GSUBR_REQ(typ) + GSUBR_OPT(typ) + GSUBR_REST(typ);
  SCM v[10];
  if (n > 10) wta(self, "internal programming error", s_gsubr_apply);
  args = CDR(args);
  for (i = 0; i < GSUBR_REQ(typ); i++) {
#ifndef RECKLESS
    if (IMP(args))
      wnargs: wta(UNDEFINED, (char *)WNA, SNAME(GSUBR_PROC(self)));
#endif
    v[i] = CAR(args);
    args = CDR(args);
  }
  for (; i < GSUBR_REQ(typ) + GSUBR_OPT(typ); i++) {
    if (NIMP(args)) {
      v[i] = CAR(args);
      args = CDR(args);
    }
    else
      v[i] = UNDEFINED;
  }
  if (GSUBR_REST(typ))
    v[i] = args;
  else
    ASRTGO(NULLP(args), wnargs);
  switch (n) {
  case 2: return (*fcn)(v[0], v[1]);
  case 3: return (*fcn)(v[0], v[1], v[2]);
  case 4: return (*fcn)(v[0], v[1], v[2], v[3]);
  case 5: return (*fcn)(v[0], v[1], v[2], v[3], v[4]);
  case 6: return (*fcn)(v[0], v[1], v[2], v[3], v[4], v[5]);
  case 7: return (*fcn)(v[0], v[1], v[2], v[3], v[4], v[5], v[6]);
  case 8: return (*fcn)(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
  case 9: return (*fcn)(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
  case 10: return (*fcn)(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9]);
  }
}
