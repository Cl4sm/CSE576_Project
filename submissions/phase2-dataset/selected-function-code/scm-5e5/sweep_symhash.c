     SCM v;
{
  SCM al, x, *lloc;
  int k = LENGTH(v);
  while (k--) {
    lloc = &(VELTS(v)[k]);
    while NIMP(al = (*lloc & ~1L)) {
      x = CAR(al);
      if (GC8MARKP(CAR(x))) {
	lloc = &(CDR(al));
	SETGCMARK(x);
      }
      else {
	*lloc = CDR(al);
	CLRGCMARK(al);		/* bucket pair to be collected by gc_sweep */
	CLRGCMARK(x);		/* value cell to be collected by gc_sweep */
	gc_syms_collected++;
      }
    }
    VELTS(v)[k] &= ~1L;		/* We may have deleted the first cell */
  }
}
