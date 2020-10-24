int sc_color_gradient_index(bool dither, const int *gradient, int y) {

   const int *gp;
   int low;
   int wid;
   int probdither;

   /*** This is the dither subsystem ***/   
   if(dither) {
      gp = gradient + 1;
      while(*gp < y) ++gp;
      low = *(gp - 1);
      wid = *gp - low;
      if(wid > 0) probdither = (0x10000 * (y - low)) / wid;
      else        probdither = 0x10000;
      if(_sc_color_gradient_rand() < probdither) return(gp - gradient);
      return(gp - gradient - 1);
   } else {
      gp = gradient + 1;
      while(*gp < y) ++gp;
      return(gp - gradient - 1);
   }

}
