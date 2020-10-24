static void egc_sweep()
{
  SCM z;
  int i;
  for (i = scm_ecache_index; i < scm_ecache_len; i++) {
    z = PTR2SCM(&(scm_ecache[i]));
    if (CONSP(z)) {
      CLRGCMARK(z);
    }
    else {
      CLRGC8MARK(z);
    }
  }
  /* Under some circumstances I don't fully understand, continuations may
     point to dead ecache cells. This prevents gc marked cells from causing
     errors during ecache gc. */
  for (i = scm_ecache_index; i--;) {
    scm_ecache[i].car = UNSPECIFIED;
    scm_ecache[i].cdr = UNSPECIFIED;
  }
}
