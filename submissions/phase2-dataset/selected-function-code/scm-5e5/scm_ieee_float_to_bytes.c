     SCM in_flt;
{
  double dbl = num2dbl(in_flt, (char *)ARG1, s_ieee_float_to_bytes);
  float flt = (float) dbl;
  SCM sbyts = make_string(MAKINUM(4), MAKICHR(0));
  char *byts = CHARS(sbyts);
  int s = flt < 0.0;
  int scl = 0x7f;
  flt = fabs(flt);
  if (0.0==flt) {
    if (s)
      byts[0] = 0x80;
    return sbyts;
  }
  else if (flt != flt) {
    byts[0] = 0x7f;
    byts[1] = 0xc0;
    return sbyts;
  }
  else goto L_scale;
 L_out:
  {
    float T_flt = 0x80 * (flt);
    int val = (int)(floor(0x80 * (flt)));
    int idx = 1;
    float flt = T_flt;
    while (!((idx) > 3)) {
      byts[idx] = val;
      {
	float T_flt = 0x100 * ((flt) - (val));
	int T_val = (int)(floor(0x100 * ((flt) - (val))));
	idx = 1 + (idx);
	flt = T_flt;
	val = T_val;
      }
    }
    byts[1] = (0x80 & (scl<<7)) | (0x7f & (((unsigned char*)(byts))[1]));
    byts[0] = (s ? 0x80 : 0) + ((scl)>>1);
    return sbyts;
  }
 L_scale:
  if (!(scl)) {
    flt = (flt)/2;
    goto L_out;
  }
  else if ((flt)>=0x10) {
    float flt16 = (flt) / 0x10;
    if ((flt16)==(flt)) {
      byts[0] = s ? 0xff : 0x7f;
      byts[1] = 0x80;
      return sbyts;
    }
    else {
      flt = flt16;
      scl = (scl) + 4;
      goto L_scale;
    }
  }
  else if ((flt) >= 2) {
    flt = (flt) / 2;
    scl = (scl) + 1;
    goto L_scale;
  }
  else if (((scl) >= 4) && ((0x10 * (flt))<1)) {
    flt = (flt) * 0x10;
    scl = (scl)+ -4;
    goto L_scale;
  }
  else if ((flt)<1) {
    flt = (flt) * 2;
    scl = (scl) + -1;
    goto L_scale;
  }
  else {
    flt = -1+(flt);
    goto L_out;
  }
}
