     scm_gra *gra;
     char *elt;
{
  int i;
  char *tmp;
  if (gra->alloclen <= gra->len) {
    sizet inc = gra->len / 5 + 1;
    sizet nlen = gra->len + inc;
    if (gra->maxlen && nlen > gra->maxlen)
      /* growerr: */ wta(MAKINUM(nlen), (char *)NALLOC, gra->what);
    /*
      SYSCALL(tmp = realloc(gra->elts, nlen*gra->eltsize););
      if (!tmp) goto growerr;
      mallocated += (nlen - gra->alloclen)*gra->eltsize;
    */
    tmp = must_realloc(gra->elts, (long)gra->alloclen*gra->eltsize,
		       (long)nlen*gra->eltsize, gra->what);
    gra->elts = tmp;
    gra->alloclen = nlen;
  }
  tmp = &gra->elts[gra->len*gra->eltsize];
  gra->len += 1;
  for (i = 0; i < gra->eltsize; i++)
    tmp[i] = elt[i];
  return gra->len - 1;
}
