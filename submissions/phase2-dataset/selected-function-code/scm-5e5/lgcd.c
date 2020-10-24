SCM lgcd(x, y)
     SCM x, y;
{
  register long u, v, k, t;
 tailrec:
  if (UNBNDP(y)) return UNBNDP(x) ? INUM0 : x;
#ifdef BIGDIG
  if (NINUMP(x)) {
    big_gcd:
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_gcd);
    if (BIGSIGN(x)) x = copybig(x, 0);
  newy:
    if (NINUMP(y)) {
      ASRTER(NIMP(y) && BIGP(y), y, ARG2, s_gcd);
      if (BIGSIGN(y)) y = copybig(y, 0);
      switch (bigcomp(x, y)) {
      case -1:
      swaprec: t = lremainder(x, y); x = y; y = t; goto tailrec;
      case  0: return x;
      case  1: y = lremainder(y, x); goto newy;
      }
      /* instead of the switch, we could just return lgcd(y, modulo(x, y)); */
    }
    if (INUM0==y) return x; goto swaprec;
  }
  if (NINUMP(y)) { t=x; x=y; y=t; goto big_gcd;}
#else
  ASRTER(INUMP(x), x, ARG1, s_gcd);
  ASRTER(INUMP(y), y, ARG2, s_gcd);
#endif
  u = INUM(x);
  if (u<0) u = -u;
  v = INUM(y);
  if (v<0) v = -v;
  else if (0==v) goto getout;
  if (0==u) {u = v; goto getout;}
  for (k = 1;!(1 & ((int)u|(int)v));k <<= 1, u >>= 1, v >>= 1);
  if (1 & (int)u) t = -v;
  else {
    t = u;
b3:
    t = SRS(t, 1);
  }
  if (!(1 & (int)t)) goto b3;
  if (t>0) u = t;
  else v = -t;
  t = u-v;
  if (t) goto b3;
  u = u*k;
getout:
  if (!POSFIXABLE(u))
#ifdef BIGDIG
    return long2big(u);
#else
    wta(x, (char *)OVFLOW, s_gcd);
#endif
  return MAKINUM(u);
}
