     int signp;
     SCM dat;
     XPoint *ipr;
     char *pos, *s_caller;
{
  SCM x, y;
  if (IMP(dat)) badarg: wta(dat, pos, s_caller);
  if (CONSP(dat)) {
    if (INUMP(CDR(dat))) {
      x = CAR(dat);
      y = CDR(dat);
    }
    else {
      ASRTGO(2==ilength(dat), badarg);
      x = CAR(dat);
      y = CAR(CDR(dat));
    }
  }
  else switch TYP7(dat) {
  default: goto badarg;
  case tc7_vector:
    ASRTGO(2==LENGTH(dat), badarg);
    x = VELTS(dat)[0];
    y = VELTS(dat)[1];
    break;
  case tc7_VfixN32: case tc7_VfixZ32:
    ASRTGO(2==LENGTH(dat), badarg);
    x = MAKINUM(((long *)VELTS(dat))[0]);
    y = MAKINUM(((long *)VELTS(dat))[1]);
    break;
  case tc7_VfixZ16:
    ASRTGO(2==LENGTH(dat), badarg);
    x = MAKINUM(((short *)VELTS(dat))[0]);
    y = MAKINUM(((short *)VELTS(dat))[1]);
    break;
  case tc7_smob:
    ASRTGO(ARRAYP(dat) && 1==ARRAY_NDIM(dat) &&
	   0==ARRAY_DIMS(dat)[0].lbnd && 1==ARRAY_DIMS(dat)[0].ubnd,
	   badarg);
      x = aref(dat, MAKINUM(0));
      y = aref(dat, MAKINUM(1));
      break;
  }
  ASRTGO(INUMP(x) && INUMP(y), badarg);
  ipr->x = INUM(x);
  ipr->y = INUM(y);
  ASRTGO((ipr->x==INUM(x)) && (ipr->y==INUM(y))
	 && (signp ? !0 : ((x >= 0) && (y >= 0))), badarg);
}
