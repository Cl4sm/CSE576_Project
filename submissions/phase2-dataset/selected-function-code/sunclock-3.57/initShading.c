void 
initShading(Context) 
struct Sundata * Context;
{
      int i;

      if (Context->flags.shading <2) 
         Context->shadefactor = 1.0;
      else {
         if (Context->flags.shading == 2)
            Context->shadefactor = 180.0/(M_PI*(SUN_APPRADIUS+atm_refraction));
         else
            Context->shadefactor = 180.0/(M_PI*(SUN_APPRADIUS+atm_diffusion));
      }

      Context->shadescale = 0.5 * ((double)Context->flags.colorscale + 0.5);

      if (Context->flags.shading == 0 || Context->flags.shading >= 4) {
         if (Context->tr1) {
            free(Context->tr1);
            Context->tr1 = NULL;
         }
      }

      if (Context->flags.shading <= 1 && Context->flags.shading >= 4) {
         if (Context->tr2) {
            free(Context->tr2);
            Context->tr2 = NULL;
         }
      }

      if (Context->flags.shading >= 1 && Context->flags.shading <= 3) {
         if (!Context->tr1)  
            Context->tr1 = (short *) 
                     salloc(Context->geom.width*sizeof(short));
         for (i=0; i< (int)Context->geom.width; i++) Context->tr1[i] = 0;
      }
      
      if (Context->flags.shading >= 2 && Context->flags.shading <= 3) {
         if (!Context->tr2)  
            Context->tr2 = (short *) 
                     salloc(Context->geom.width*sizeof(short));
         for (i=0; i< (int)Context->geom.width; i++) 
            Context->tr2[i] = -1; 
      }

      Context->south = -1;
}       
