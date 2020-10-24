     SCM sn;
     SCM slen;
{
  ASRTER(INUMP(slen), slen, ARG2, s_integer_to_bytes);
  {
    int len = INUM(slen);
    SCM sbyts = make_string(scm_iabs(slen), MAKICHR(0));
    char *byts = CHARS(sbyts);
    if (INUMP(sn)) {
      int idx = -1 + (abs(len));
      long n = num2long(sn, (char *)ARG1, s_integer_to_bytes);
      if ((0 > n) && (0 > len)) {
	long res = -1 - n;
	while (!(0 > idx)) {
	  byts[idx--] = 0xFF ^ (res % 0x100);
	  res = res>>8;
	}
      }
      else {
	unsigned long res = n;
	while (!(0 > idx)) {
	  byts[idx--] = res % 0x100;
	  res = res>>8;
	}
      }
    } else {
      ASRTER(NIMP(sn) && BIGP(sn), sn, ARG1, s_integer_to_bytes);
      {
	BIGDIG *digs = BDIGITS(sn), borrow = 1;
	sizet ndigs = NUMDIGS(sn);
	int iu = 0, id = abs(len) - 1;
	unsigned long dig;
	if ((0 > len) && (TYP16(sn)==tc16_bigneg))
	  for (; 0 <= id ; iu++) {
	    sizet j = sizeof(BIGDIG);
	    dig = (iu < ndigs) ? digs[iu] : 0;
	    dig = dig ^ ((1 << (8 * sizeof(BIGDIG))) - 1);
	    /* printf("j = %d; id = %d; iu = %d; dig = %04x; borrow = %d\n", j, id, iu, dig, borrow); */
	    for (; 0 < j-- && 0 <= id;) {
	      /* printf("byts[%d] = %02x\n", id, 0xFF & dig); */
	      int dg = (0xFF & dig) + borrow;
	      borrow = dg >> 8;
	      ((unsigned char *)byts)[id--] = dg;
	      dig = (dig)>>8;
	    }
	  }
	else
	  for (; 0 <= id ; iu++) {
	    BIGDIG dig = (iu < ndigs) ? digs[iu] : 0;
	    sizet j = sizeof(BIGDIG);
	    /* printf("j = %d; id = %d; iu = %d; dig = %04x\n", j, id, iu, dig); */
	    for (; 0 < j-- && 0 <= id;) {
	      /* printf("byts[%d] = %02x\n", id, 0xFF & dig); */
	      ((unsigned char *)byts)[id--] = 0xFF & dig;
	      dig = (dig>>8);
	    }
	  }
      }
    }
    return sbyts;
  }
}
