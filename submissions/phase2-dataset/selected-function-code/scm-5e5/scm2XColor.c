int scm2XColor(s_dat, xclr)
     SCM s_dat;
     XColor *xclr;
{
  SCM dat = s_dat;
  unsigned int ura[3];
  int idx;
/*    if (INUMP(dat)) { */
/*      xclr->red   = (dat>>16 & 0x00ff) * 0x0101; */
/*      xclr->green = (dat>>8  & 0x00ff) * 0x0101; */
/*      xclr->blue  = (dat     & 0x00ff) * 0x0101; */
/*    } */
/*    else */
  if (IMP(dat)) return 0;
  else if (3==ilength(dat))
    for (idx = 0; idx < 3; idx++) {
      SCM clr = CAR(dat);
      if (!INUMP(clr)) return 0;
      ura[idx] = INUM(clr);
      dat = CDR(dat);
    }
  else if (VECTORP(dat) && (3==LENGTH(dat)))
    for (idx = 0; idx < 3; idx++) {
      if (!INUMP(VELTS(dat)[idx])) return 0;
      ura[idx] = INUM(VELTS(dat)[idx]);
    }
  else return 0;
  xclr->red = ura[0];
  xclr->green = ura[1];
  xclr->blue = ura[2];
  return !0;
}
