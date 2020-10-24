/* sc_window_flame_map_gtk */

   double dist;
   int *flamemap;
   int maxdelta;
   int effect;
   int bigrad;
   int delta;
   int theta;
   int size;
   int rad;
   int max;
   int idx;
   int x;
   int y;

   size = SC_NAPALM_FLAME_RAD * 2 + 1;
   flamemap = (int *)malloc(sizeof(int) * SQR(size));
   if(flamemap == NULL) return(NULL);

   memset((char *)flamemap, 0xff, sizeof(int) * SQR(size));

   max = c->colors->gradsize[SC_GRAD_FLAMES] - 1;

   rad = SC_NAPALM_FLAME_RAD / 2 + sys_lrand(SC_NAPALM_FLAME_RAD / 2) + 1;
   theta = 60;

   bigrad = rad / sin(theta * M_PI / 180);
   maxdelta = (1 - cos(theta * M_PI / 180)) * bigrad;

   effect = 0;
   for(y = -rad; y < rad; ++y) {
      delta = sqrt(SQR(bigrad) - SQR(y)) - cos(theta * M_PI / 180) * bigrad;
      if(effect - delta < -SC_NAPALM_FLAME_RAD) effect = -SC_NAPALM_FLAME_RAD + delta;
      if(effect + delta >  SC_NAPALM_FLAME_RAD) effect =  SC_NAPALM_FLAME_RAD - delta;
      for(x = -delta; x < delta; ++x) {
         idx = (y + rad) * size + x + effect + SC_NAPALM_FLAME_RAD;
         dist = sqrt(SQR((double)x / maxdelta) + SQR((double)(y + rad) / (2 * rad)));
         *(flamemap + idx) = max - dist * max / M_SQRT2;
      }
      switch(sys_lrand(5)) {
         case 0:  ++effect; break;
         case 1:  --effect; break;
      }
   }

   return(flamemap);

}
