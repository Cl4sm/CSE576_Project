SCM scm_bytes_to_integer(sbyts, sn)
     SCM sbyts;
     SCM sn;
{
  long n = INUM(sn);
  if (!(n)) return INUM0;
  {
    int cnt = abs(n);
    char *byts = get_bytes(sbyts, cnt, s_bytes_to_integer);
    int iu = 0, id = cnt - sizeof(BIGDIG);
    sizet ndigs = (cnt + sizeof(BIGDIG) - 1) / sizeof(BIGDIG);
    int negp = (0x80 & byts[0]) && (0 > n);
    SCM retval = mkbig(ndigs, negp);
    BIGDIG *digs = BDIGITS(retval), carry = 1;
    if (negp)
      for (; iu < ndigs; iu++) {
	int j = 0;
	unsigned long dig = 0;
	for (; j < sizeof(BIGDIG); j++) {
	  dig = (dig<<8) +
	    (0xFF ^ ((id + j >= 0) ? (((unsigned char *)byts)[id + j]) : 255));
	  /* printf("byts[%d + %d] = %lx\n", id, j, 0xFF & dig); */
	}
	dig = dig + carry;
	digs[iu] = dig;
	carry = dig >> (8 * sizeof(BIGDIG));
	/* printf("id = %d; iu = %d; dig = %04lx\n", id, iu, dig); */
	id = id - sizeof(BIGDIG);
      } else
      for (; iu < ndigs; iu++) {
	int j = 0;
	BIGDIG dig = 0;
	for (; j < sizeof(BIGDIG); j++) {
	  dig = (dig<<8) +
	    ((id + j >= 0) ? (((unsigned char *)byts)[id + j]) : 0);
	}
	digs[iu] = dig;
	/* printf("id = %d; iu = %d; dig = %04x\n", id, iu, dig); */
	id = id - sizeof(BIGDIG);
      }
    return normbig(retval);
  }
}
