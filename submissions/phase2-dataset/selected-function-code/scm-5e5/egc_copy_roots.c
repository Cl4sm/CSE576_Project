{
  SCM *roots = &(scm_egc_roots[scm_egc_root_index]);
  SCM e, x;
  int len = sizeof(scm_egc_roots)/sizeof(SCM) - scm_egc_root_index ;
  if (!(len>=0 && len <= sizeof(scm_egc_roots)/sizeof(SCM)))
    wta(MAKINUM(scm_egc_root_index), "egc-root-index", "corrupted");
  while (len--) {
    x = roots[len];
    if (IMP(x)) continue;
    switch TYP3(x) {
    clo:
    case tc3_closure:
      e = ENV(x);
      if (NIMP(e) && ECACHEP(e)) {
	egc_copy(&e);
	CDR(x) = (6L & CDR(x)) | e;
      }
      break;
    case tc3_cons_imcar:
    case tc3_cons_nimcar:	/* These are environment frames that have
				   been destructively altered by DEFINE or
				   LETREC.  This is only a problem if a
				   non-cache cell was made to point into the
				   cache. */
      if (ECACHEP(x)) break;
      e = CAR(x);
      if (NIMP(e) && ECACHEP(e))
	egc_copy(&(CAR(x)));
      break;
    default:
      if (tc7_contin==TYP7(x)) {
	egc_copy_locations(CONT(x)->other.stkframe, 2);
#ifndef CHEAP_CONTINUATIONS
	x = CONT(x)->other.estk;
	egc_copy_stack(x, LENGTH(x));
#endif
	break;
      }
      if (tc16_env==CAR(x)) {
	e = CDR(x);
	if (NIMP(e) && ECACHEP(e))
	  egc_copy(&(CDR(x)));
	break;
      }
      if (tc16_promise==CAR(x)) {
	x = CDR(x);
	goto clo;
      }
    }
  }
  scm_egc_root_index = sizeof(scm_egc_roots)/sizeof(SCM);
}
