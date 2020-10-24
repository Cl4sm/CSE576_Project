     SCM w, x, y;
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
   CAR(z1) = x;
   CDR(z1) = y;
   z2 = PTR2SCM(&(scm_ecache[--i]));
   CAR(z2) = w;
   CDR(z2) = z1;
   scm_env_tmp = z2;
   scm_ecache_index = i;
}
