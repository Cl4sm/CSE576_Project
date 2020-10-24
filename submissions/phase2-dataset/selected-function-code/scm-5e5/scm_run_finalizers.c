void scm_run_finalizers(exiting)
     int exiting;
{
  SCM f;
  if (exiting) {		/* run all finalizers, we're going home. */
    DEFER_INTS;
    while NIMP(gc_finalizers) {
      f = CAR(gc_finalizers);
      CAR(f) = CDR(f);
      CDR(f) = gc_finalizers_pending;
      gc_finalizers_pending = f;
      gc_finalizers = CDR(gc_finalizers);
    }
    ALLOW_INTS;
  }
  while (!0) {
    DEFER_INTS;
    if (NIMP(gc_finalizers_pending)) {
      f = CAR(gc_finalizers_pending);
      gc_finalizers_pending = CDR(gc_finalizers_pending);
    }
    else f = BOOL_F;
    ALLOW_INTS;
    if (IMP(f)) break;
    apply(f, EOL, EOL);
  }
}
