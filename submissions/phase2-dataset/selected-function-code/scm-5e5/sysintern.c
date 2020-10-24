     const char *name;
     SCM val;
{
  SCM lsym, z;
  sizet len = strlen(name);
  register sizet i = len;
  register unsigned char *tmp = (unsigned char *)name;
  sizet hash = strhash(tmp, i, (unsigned long)symhash_dim);
  for (lsym = VELTS(symhash)[hash];NIMP(lsym);lsym = CDR(lsym)) {
    z = CAR(lsym);
    z = CAR(z);
    tmp = UCHARS(z);
    if (LENGTH(z) != len) goto trynext;
    for (i = len;i--;) if (((unsigned char *)name)[i] != tmp[i]) goto trynext;
    lsym = CAR(lsym);
    if (!UNBNDP(val)) CDR(lsym) = val;
    else if (UNBNDP(CDR(lsym)) && tc7_msymbol==TYP7(CAR(lsym)))
      scm_gc_protect(lsym);
    return lsym;
  trynext: ;
  }
  NEWCELL(lsym);
  SETLENGTH(lsym, len, tc7_ssymbol);
  SETCHARS(lsym, name);
  lsym = cons(lsym, val);
  z = cons(lsym, UNDEFINED);
  CDR(z) = VELTS(symhash)[hash];
  VELTS(symhash)[hash] = z;
  return lsym;
}
