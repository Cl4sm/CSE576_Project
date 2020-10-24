{
  /* 40 and 10 below are adjustable parameters:  the number of frames
     in a stack segment, and the number of frames to overlap between
     stack segments. */
  sizet size = 40 * SCM_ESTK_FRLEN + SCM_ESTK_BASE + 1;
  sizet overlap = 10*SCM_ESTK_FRLEN;
  SCM estk = make_stk_seg(size, UNDEFINED);
  SCM *newv, *oldv;
  sizet i, j;
  newv = VELTS(estk);
  oldv = VELTS(scm_estk);
  j = scm_estk_ptr - oldv + SCM_ESTK_FRLEN - overlap;
  SCM_ESTK_PARENT(estk) = scm_estk;
  SCM_ESTK_PARENT_WRITABLEP(estk) = BOOL_T;
  SCM_ESTK_PARENT_INDEX(estk) = MAKINUM(j - SCM_ESTK_FRLEN);
  for (i = SCM_ESTK_BASE; i < SCM_ESTK_BASE + overlap; i++, j++) {
    newv[i] = oldv[j];
    oldv[j] = BOOL_F;
  }
  scm_estk = estk;
  scm_estk_ptr = &(newv[SCM_ESTK_BASE + overlap]);
  scm_estk_size += size + 0L;
  /*  growth_mon(s_estk, scm_estk_size, "locations", !0); */
}
