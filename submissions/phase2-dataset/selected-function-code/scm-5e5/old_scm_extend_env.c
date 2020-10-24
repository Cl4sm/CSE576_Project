     SCM names;
{
   SCM z1, z2;
   register int i;
   DEFER_INTS_EGC;
   i = scm_ecache_index;
   if (2>i) {
     scm_egc();
     i = scm_ecache_index;
   }
   z1 = PTR2SCM(&(scm_ecache[--i]));
   CAR(z1) = names;
   CDR(z1) = scm_env_tmp;
   z2 = PTR2SCM(&(scm_ecache[--i]));
   CAR(z2) = z1;
   CDR(z2) = scm_env;
   scm_env = z2;
   scm_ecache_index = i;
}
