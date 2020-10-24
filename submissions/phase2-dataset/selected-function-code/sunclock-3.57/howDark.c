howDark(Context, i, j)
struct Sundata * Context;
int i, j;
{
      double light;
      int k;

      if (Context->flags.shading == 0) {
	 return -1;
      }

      light = Context->daywave[i] * Context->cosval[j] + Context->sinval[j];

      if (Context->flags.shading == 1) {
         if (light >= 0) k = -1; else k = 0;
      } else {
         if (Context->flags.shading<=3 || 
             (Context->flags.shading==4 && light<0))
             light *= Context->shadefactor;
         k = (int) ((1.0+light)*Context->shadescale);
         if (k < 0) k = 0;
         if (k >= Context->flags.colorscale) k = - 1;
      }
      return k;
}
