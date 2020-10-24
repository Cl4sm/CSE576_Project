void scm_env_v2lst(argc, argv)
     long argc;
     SCM *argv;
{
   SCM z1, z2;
   register int i;
   DEFER_INTS_EGC;
   i = scm_ecache_index;
   if (argc>i) {
     scm_egc();
     i = scm_ecache_index;
   }
   z1 = z2 = scm_env_tmp;	/* set z1 just in case argc is zero */
   while (argc--) {
     z1 = PTR2SCM(&(scm_ecache[--i]));
     CAR(z1) = argv[argc];
     CDR(z1) = z2;
     z2 = z1;
   }
   scm_env_tmp = z1;
   scm_ecache_index = i;
}
