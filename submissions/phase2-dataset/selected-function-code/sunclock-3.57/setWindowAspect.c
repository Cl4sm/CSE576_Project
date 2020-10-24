struct Sundata * Context;
{
   unsigned int b, i, j;
   int change;
   struct Geometry *       Geom;
   double f;

   if (Context->newzoom.mode <= 1)
      f = 1.0;
   else
      f = sin(M_PI*Context->newzoom.fdy);

   change = 0;
   i = Context->geom.width;

   j = (unsigned int) (0.5 + Context->newzoom.fx * 
          (double)Context->geom.width / ( 2.0 * Context->newzoom.fy * f) );
   b = DisplayHeight(dpy,scr) - Context->hstrip - vert_drift - 5;
   if (j<=0) j=1;
   if (j>b) {
      i = (Context->geom.width * b) / j;
      j = b;
   }

   Geom = (Context->wintype)? &MapGeom : &ClockGeom;

   if (j<Geom->h_mini) {
      i = (i * Geom->h_mini)/j;
      j = Geom->h_mini;
   }
  
   if (Context->geom.width != i) {
      change = 1;
      Context->geom.width = Geom->width = i;
   }
   if (Context->geom.height != j) {
      change = 1;
      Context->geom.height = Geom->height = j;
   }

   if (change && verbose)
      fprintf(stderr, "Resizing window to width = %d , height = %d\n", i, j); 

   return change;
}
