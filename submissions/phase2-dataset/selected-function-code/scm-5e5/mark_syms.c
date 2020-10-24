static void mark_syms(v)
     SCM v;
{
  SCM x, al;
  int k = LENGTH(v);
  while (k--)
    for (al = VELTS(v)[k]; NIMP(al); al = GCCDR(al)) {
      /* If this bucket has already been marked, then something is wrong.  */
      ASRTER(!GCMARKP(al), al, s_bad_type, s_gc_sym);
      x = CAR(al);
      SETGCMARK(al);		/* Do mark bucket list */
# ifdef CAREFUL_INTS
      ASRTER(NIMP(x) && NIMP(CAR(x)) && !GCMARKP(x), x, s_bad_type, s_gc_sym);
      ASRTER(!GC8MARKP(CAR(x)) && !(CHARS(CAR(x))[LENGTH(CAR(x))]),
	     CAR(x), s_wrong_length, s_gc_sym);
      ASRTER(strhash(UCHARS(CAR(x)), (sizet)LENGTH(CAR(x)),
		     (unsigned long)symhash_dim)==k,
	     CAR(x), "bad hash", s_gc_sym);
# endif
      if (UNDEFINED==CDR(x) && tc7_msymbol==TYP7(CAR(x)))
	goto used;		/* Don't mark symbol.  */
      SETGC8MARK(CAR(x));
    used:
      /* SETGCMARK(x) */;	/* Don't mark value cell.  */
      /* We used to mark the value cell, but value cells get returned
	 by calls to intern().  This caused a rare GC leak which only
	 showed up in large programs. */
    }
  SETGC8MARK(v);		/* Mark bucket vector.  */
}
