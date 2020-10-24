     SCM sbyts;
{
  char *byts = get_bytes(sbyts, 4, s_bytes_to_ieee_float);
  int len = LENGTH(sbyts);
  int s = (1<<(7)) & ((((unsigned char*)(byts))[0]));
  int e = ((0x7f&((((unsigned char*)(byts))[0])))<<1)
    + ((0x80&((((unsigned char*)(byts))[1])))>>7);
  float f = (((unsigned char*)(byts))[ -1 + (len)]);
  int idx = -2 + (len);
  while (!((idx)<=1)) {
    {
      int T_idx = -1 + (idx);
      f = ((((unsigned char*)(byts))[idx])) + ((f) / 0x100);
      idx = T_idx;
    }
  }
  f = ((0x7f&((((unsigned char*)(byts))[1]))) + ((f) / 0x100)) / 0x80;
  if ((0<(e))
      && ((e)<0xff))
    return makdbl(ldexpf((s ? -1 : 1) * (1 + (f)), (e) - 0x7f), 0.0);
  else if (!(e))
    if (!(f)) return flo0;
    else return makdbl(ldexpf((s ? -1 : 1) * (f), -126), 0.0);
  else if (f)
    return scm_narn;
  else return makdbl((s ? -(1.0) : 1.0) / 0.0, 0.0);
}
