static int ramap(ra0, proc, ras)
     SCM ra0, proc, ras;
{
  SCM heap_ve, auto_rav[5], auto_argv[5];
  SCM *rav = &auto_rav[0], *argv = &auto_argv[0];
  long argc = ilength(ras);
  long i, k, inc, n, base;
  scm_protect_temp(&heap_ve);
  if (argc >= 5) {
    heap_ve = make_vector(MAKINUM(2*argc), BOOL_F);
    rav = VELTS(heap_ve);
    argv = &(rav[argc]);
  }
  for (k = 0; k < argc; k++) {
    rav[k] = CAR(ras);
    ras = CDR(ras);
  }
  i = ARRAY_DIMS(ra0)->lbnd;
  inc = ARRAY_DIMS(ra0)->inc;
  n = ARRAY_DIMS(ra0)->ubnd;
  base = ARRAY_BASE(ra0) - i*inc;
  ra0 = ARRAY_V(ra0);
  for (; i <= n; i++) {
    for (k = 0; k < argc; k++)
      argv[k] = aref(rav[k], MAKINUM(i));
    aset(ra0, scm_cvapply(proc, argc, argv), MAKINUM(i*inc + base));
  }
  return 1;
}
