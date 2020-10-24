struct Sundata * Context;
int mode;
{
   double a, b, f, p;
   int change;

   checkZoomSettings(&Context->newzoom);

   if (memcmp(&Context->newzoom, &Context->zoom, sizeof(ZoomSettings)))
      Context->oldzoom = Context->zoom;

   if (!Context->newzoom.mode) return 0;

   a = Context->newzoom.fx;
   b = Context->newzoom.fy;
   change = 0;

   if (Context->newzoom.mode == 1)
      f = 1.0;
   else
      f = (double)Context->geom.width / 
          ((double)Context->geom.height *2.0 * sin(M_PI*Context->newzoom.fdy));

   if (mode == 3) {
      p = sqrt( Context->newzoom.fx * Context->newzoom.fy / f);
      Context->newzoom.fx = p;
      Context->newzoom.fy = p * f;
      f = 0;
      mode = 1;
   }

   if (mode == 1) {
      if (f == 0) 
          mode = 2;
      else
          Context->newzoom.fx = Context->newzoom.fy / f;
      if (Context->newzoom.fx < 1.0) {
	  Context->newzoom.fy = Context->newzoom.fy / Context->newzoom.fx;
	  Context->newzoom.fx = 1.0;
      }
      if (Context->newzoom.fx > 100.0) {
	  Context->newzoom.fy = 100.0 * Context->newzoom.fy / Context->newzoom.fx;
	  Context->newzoom.fx = 100.0;
      }
   }

   if (mode == 2) {
      if (f!=0) 
          Context->newzoom.fy = Context->newzoom.fx * f;
      if (Context->newzoom.fy < 1.0) {
	  Context->newzoom.fx = Context->newzoom.fx / Context->newzoom.fy;
	  Context->newzoom.fy = 1.0;
      }
      if (Context->newzoom.fy > 100.0) {
	  Context->newzoom.fx = 100.0 * Context->newzoom.fx / Context->newzoom.fy;
	  Context->newzoom.fy = 100.0;
      }
   }

   if (fabs(a-Context->newzoom.fx) > 1E-4) {
      zoom_mode |= 10;
      change = 1;
   } else
      Context->newzoom.fx = a;

   if (fabs(b-Context->newzoom.fy) > 1E-4) {
      zoom_mode |= 12;
      change = 1;
   } else
      Context->newzoom.fy = b;
    
   if (verbose && change)
      fprintf(stderr, "Adjusting zoom area aspect (%.2f , %.2f) --> "
            "(%.2f , %.2f)\n", a, b, Context->newzoom.fx , Context->newzoom.fy);

   return change;
}
