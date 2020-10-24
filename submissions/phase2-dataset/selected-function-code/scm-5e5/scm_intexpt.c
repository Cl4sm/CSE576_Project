     SCM z1, z2;
{
  SCM acc = MAKINUM(1L);
  int recip = 0;
#ifdef FLOATS
  double dacc, dz1;
#endif
  if (INUM0==z2) return sum(acc, product(z1, INUM0));
  ASRTER(INUMP(z2), z2, ARG2, s_intexpt);
  if (acc==z1) return z1;
  if (MAKINUM(-1L)==z1) return BOOL_F==evenp(z2)?z1:acc;
  z2 = INUM(z2);
  if (z2 < 0) {
    z2 = -z2;
    recip = 1; /*    z1 = divide(z1, UNDEFINED); */
  }
  if (INUMP(z1)) {
    long tmp, iacc = 1, iz1 = INUM(z1);
#ifdef FLOATS
    if (recip) { dz1 = iz1; goto flocase; }
#endif
    while (1) {
      if (0==z2) {
	acc = long2num(iacc);
	break;
      }
      if (0==iz1)
	if (0==recip) return z1;
	else goto overflow;
      if (1==z2) {
	tmp = iacc*iz1;
	if (tmp/iacc != iz1) {
	overflow:
	  z1 = long2num(iz1);
	  acc = long2num(iacc);
	  ASRTGO(NFALSEP(z1) && NFALSEP(acc), errout);
	  goto gencase;
	}
	acc = long2num(tmp);
	break;
      }
      if (z2 & 1) {
	tmp = iacc*iz1;
	if (tmp/iacc != iz1) goto overflow;
	iacc = tmp;
	z2 = z2 - 1;		/* so jumping to gencase works  */
      }
      tmp = iz1*iz1;
      if (tmp/iz1 != iz1) goto overflow;
      iz1 = tmp;
      z2 >>= 1;
    }
#ifndef RECKLESS
    if (FALSEP(acc))
      errout: wta(UNDEFINED, (char *)OVFLOW, s_intexpt);
#endif
    goto ret;
  }
  ASRTER(NIMP(z1), z1, ARG1, s_intexpt);
#ifdef FLOATS
  if (REALP(z1)) {
    dz1 = REALPART(z1);
  flocase:
    dacc = 1.0;
    while(1) {
      if (0==z2) break;
      if (1==z2) {dacc = dacc*dz1; break;}
      if (z2 & 1) dacc = dacc*dz1;
      dz1 = dz1*dz1;
      z2 >>= 1;
    }
    return makdbl(recip ? 1.0/dacc : dacc, 0.0);
  }
#endif
 gencase:
  while(1) {
    if (0==z2) break;
    if (1==z2) {acc = product(acc, z1); break;}
    if (z2 & 1) acc = product(acc, z1);
    z1 = product(z1, z1);
    z2 >>= 1;
  }
 ret: return recip ? divide(acc, UNDEFINED) : acc;
}
