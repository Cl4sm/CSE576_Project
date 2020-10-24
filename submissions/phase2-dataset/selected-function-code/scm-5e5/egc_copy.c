static void egc_copy(px)
     SCM *px;
{
  SCM z, x = *px;
  do {
    if (tc_broken_heart==CAR(x)) {
      *px = CDR(x);
      return;
    }
    if (IMP(freelist)) wta(freelist, "empty freelist", "ecache gc");
    z = freelist;
    freelist = CDR(freelist);
    ++cells_allocated;
    CAR(z) = CAR(x);
    CDR(z) = CDR(x);
    CAR(x) = (SCM)tc_broken_heart;
    CDR(x) = z;
    *px = z;
    x = CAR(z);
    if (NIMP(x) && ECACHEP(x))
      egc_copy(&(CAR(z)));
    px = &(CDR(z));
    x = *px;
  } while (NIMP(x) && ECACHEP(x));
}
