     SCM k, fill;
{
  SCM v;
  register long i;
  register SCM *velts;
#ifdef SHORT_SIZET
  ASRTER(INUMP(k), k, ARG1, s_make_vector);
#else
  ASRTER(INUMP(k) && (!(~LENGTH_MAX & INUM(k))), k, ARG1, s_make_vector);
#endif
  if (UNBNDP(fill)) fill = UNSPECIFIED;
  i = INUM(k);
  DEFER_INTS;
  v = must_malloc_cell(i ? i*sizeof(SCM) : 1L,
		       MAKE_LENGTH(i, tc7_vector), s_vector);
  velts = VELTS(v);
  while(--i >= 0) (velts)[i] = fill;
  ALLOW_INTS;
  return v;
}
