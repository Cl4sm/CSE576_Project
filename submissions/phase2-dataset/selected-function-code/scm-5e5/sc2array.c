     SCM s, ra, prot;
{
  SCM res;
  ASRTER(NIMP(ra), ra, ARG2, s_sc2array);
  if (ARRAYP(ra)) {
    int k = ARRAY_NDIM(ra);
    res = make_ra(k);
    while (k--) {
      ARRAY_DIMS(res)[k].ubnd = ARRAY_DIMS(ra)[k].ubnd;
      ARRAY_DIMS(res)[k].lbnd = ARRAY_DIMS(ra)[k].lbnd;
      ARRAY_DIMS(res)[k].inc = 0;
    }
    ra = ARRAY_V(ra);
  }
  else {
    ASRTER(BOOL_T==arrayp(ra, UNDEFINED), ra, ARG2, s_sc2array);
    res = make_ra(1);
    ARRAY_DIMS(res)->ubnd = LENGTH(ra) - 1;
    ARRAY_DIMS(res)->lbnd = 0;
    ARRAY_DIMS(res)->inc = 0;
  }
  if (NIMP(s) && ARRAYP(s) && 0==ARRAY_NDIM(s)) {
    ARRAY_BASE(res) = ARRAY_BASE(s);
    ARRAY_V(res) = ARRAY_V(s);
    return res;
  }
  ARRAY_BASE(res) = 0;
  ARRAY_V(res) = make_uve(1L, NULLP(prot) ? array_prot(ra) : CAR(prot));
  switch TYP7(ARRAY_V(res)) {
  case tc7_vector:
    break;
  case tc7_Vbool:
    if (BOOL_T==s || BOOL_F==s) break;
    goto mismatch;
  case tc7_string:
    if (ICHRP(s)) break;
    goto mismatch;
  case tc7_VfixN32:
    if (INUMP(s) && INUM(s)>=0) break;
#ifdef BIGDIG
    if (NIMP(s) && tc16_bigpos==TYP16(s) && NUMDIGS(s)<=DIGSPERLONG) break;
#endif
    goto mismatch;
  case tc7_VfixZ32:
    if (INUMP(s)) break;
#ifdef BIGDIG
    if (NIMP(s) && BIGP(s) && NUMDIGS(s)<=DIGSPERLONG) break;
#endif
    goto mismatch;
#ifdef FLOATS
  case tc7_VfloR32:
  case tc7_VfloR64:
    if (NUMBERP(s) && !(NIMP(s) && CPLXP(s))) break;
    goto mismatch;
  case tc7_VfloC32:
  case tc7_VfloC64:
    if (NUMBERP(s)) break;
    goto mismatch;
#endif
  mismatch: ARRAY_V(res) = make_vector(MAKINUM(1), s);
    return res;
  }
  aset(ARRAY_V(res), s, INUM0);
  return res;
}
