     SCM ra0, proc, ras;
{
  SCM heap_ve, auto_rav[5], auto_argv[5];
  SCM *rav = &auto_rav[0], *argv = &auto_argv[0];
  long argc = ilength(ras) + 1;
  long i, k, n;
  scm_protect_temp(&heap_ve);
  if (argc >= 5) {
    heap_ve = make_vector(MAKINUM(2*argc), BOOL_F);
    rav = VELTS(heap_ve);
    argv = &(rav[argc]);
  }
  rav[0] = ra0;
  for (k = 1; k < argc; k++) {
    rav[k] = CAR(ras);
    ras = CDR(ras);
  }
  i = ARRAY_DIMS(ra0)->lbnd;
  n = ARRAY_DIMS(ra0)->ubnd;
  for (; i <= n; i++) {
    for (k = 0; k < argc; k++)
      argv[k] = aref(rav[k], MAKINUM(i));
    scm_cvapply(proc, argc, argv);
  }
  return 1;
}
