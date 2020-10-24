void
moveNightArea(Context)
struct Sundata * Context;
{
      int i, j, k, l, jmin, jmax, j0;
      int midcolor, south, north;
      short tr1, tr2;
      double shift, shiftp, quot, cd, sd;
      double f1, f2, f3; 

      Context->flags.hours_shown = 0;
      if (!Context->flags.shading) return;

      f1 = M_PI/((double) Context->zoom.height);
      f2 = ((double) Context->zoom.height)/M_PI;
      f3 = 1E-8 * f2;

      shift = f1 * (double)Context->zoom.dy;
      shiftp = 0.5*(Context->zoom.height+1) - (double) Context->zoom.dy;
      quot = torad(Context->sundec);
      cd = cos(quot);
      sd = sin(quot);
      if (quot>0) south = 0; else south = -1;
      north = -1-south;

      quot = 2.0*M_PI/(double)Context->zoom.width;
      for (i = 0; i < (int)Context->geom.width; i++)
         Context->daywave[i] = cos(quot *(((double)i)-Context->fnoon));

      for (j = 0; j < (int)Context->geom.height; j++) {
         quot = shift + f1 * (double)j;
         Context->cosval[j] = sin(quot)*cd;
         Context->sinval[j] = cos(quot)*sd;
      }

      /* Shading = 1 uses tr1 as j-integer value of transition day/night */
      /* Context->south means color value near South pole */
      /* which is updated as south, with north = -1-south = opposite color */

      if (Context->flags.shading == 1) {
         for (i = 0; i < (int)Context->geom.width; i++) {
            if (fabs(sd)>f3)
               tr1 = (int) (shiftp+f2*atan(Context->daywave[i]*cd/sd));
            else
               tr1 = 0;
            if (tr1<0) tr1 = 0;     
            if (tr1>(int)Context->geom.height) tr1 = (int)Context->geom.height;
            if (south==Context->south) {
               for (j = tr1; j<(int)Context->tr1[i]; j++)
                     DarkenPixel(Context, i, j, south);
               for (j = (int)Context->tr1[i]; j<tr1; j++)
                     DarkenPixel(Context, i, j, north);
            } else {
               if (tr1 <= (int)Context->tr1[i]) {
                  for (j = 0; j<tr1; j++)
                     DarkenPixel(Context, i, j, north);
                  for (j = (int)Context->tr1[i]; 
                           j<(int)Context->geom.height; j++)
                     DarkenPixel(Context, i, j, south);
               } else {
                  for (j = 0; j<(int)Context->tr1[i]; j++)
                     DarkenPixel(Context, i, j, north);
                  for (j = tr1; j<(int)Context->geom.height; j++)
                     DarkenPixel(Context, i, j, south);
               }
            }
            Context->tr1[i] = tr1;
         }
         Context->south = south;
         return;
      }

      /* Shading = 4,5 are quite straightforward... compute everything! */

      if (Context->flags.shading >= 4) {
         for (i = 0; i < (int)Context->geom.width; i++)
            for (j = 0; j< (int)Context->geom.height; j++) {
               DarkenPixel(Context, i, j, howDark(Context, i, j));
            }
         return;
      }

      /* Shading = 2,3 uses both tr1 and tr2 and is very tricky... */
      /* If both tr1,tr2 >=0 then normal transition 
            day -> shadow -> night  (or night -> shadow -> day)
         Otherwise we have an "exceptional transition"
            shadow near North pole -> (day or night) -> shadow near South pole
         Day or night is encoded midcolor, determined as follow:
              if tr1<0 then midcolor = Context->south
              if tr2<0 then midcolor = -1-Context->south (opposite color)
         Renormalize integres by  tr1=-2-tr1 if <0 tr2=-2-tr2 if <0.
         Then tr1>tr2 are the limits for the interval where color=midcolor */

      for (i = 0; i < (int)Context->geom.width; i++) {
         if (fabs(sd)>f3)
            j0 = (int) (shiftp+f2*atan(Context->daywave[i]*cd/sd));
         else
            j0 = 0;
         if (j0<0) j0 = 0;
         if (j0>(int)Context->geom.height) j0 = (int)Context->geom.height;
         
         tr1 = 0;
         tr2 = (short)(Context->geom.height-1);
         midcolor = -2;
         if (Context->tr1[i] < 0) {
            Context->tr1[i] = -Context->tr1[i]-2;
            midcolor = Context->south;
         }
         if (Context->tr2[i] < 0 && midcolor==-2) {
            Context->tr2[i] = -Context->tr2[i]-2;
            midcolor = -1-Context->south;
         }

         for (j=j0; j<(int)Context->geom.height; j++) {
            k = howDark(Context, i, j);
            if (k!=south) 
               DarkenPixel(Context, i, j, k);
            else {
               tr2 = (short)(j-1);
               jmax = (int)Context->geom.height-1;
               if (j<jmax && howDark(Context, i, jmax) != south) {
                  for (l=jmax; l>tr2 ; l--) {
                     k = howDark(Context, i, l);
                     if (k!=south) 
                        DarkenPixel(Context, i, l, k);
                     else {
                        jmax = l;
                        break;
                     }
                  }
                  tr1 = (short)(-jmax-1);
               }
               if (Context->tr1[i]<=Context->tr2[i]) {
                  if (Context->south == south) {
                     if ((int)Context->tr2[i]<jmax) jmax=(int)Context->tr2[i];
                  } else {
                     if ((int)Context->tr1[i]>j) j=(int)Context->tr1[i];
                  }
               } else {
                  if (midcolor == south) {
                     for (l=j; l<=jmax && l<= (int)Context->tr2[i]; l++)
                         DarkenPixel(Context, i, l, south);
                     for (l=jmax; l>=j && l>= (int)Context->tr1[i]; l--)
                         DarkenPixel(Context, i, l, south);
                     break;
                  }
               }
               for (l=j; l<=jmax; l++) DarkenPixel(Context, i, l, south);
               break;
            }
         }

         for (j=j0-1; j>=0; j--) {
            k = howDark(Context, i, j);
            if (k!=north) 
               DarkenPixel(Context, i, j, k);
            else {
               tr1 = (short) j+1;
               jmin = 0;
               if (j>0 && howDark(Context, i, 0) != north) {
                  for (l=0; l<tr1; l++) {
                     k = howDark(Context, i, l);
                     if (k!=north) 
                        DarkenPixel(Context, i, l, k);
                     else {
                        jmin = l;
                        break;
                     }
                  }
                  tr2 = (short)(-jmin-1);
               }
               if (Context->tr1[i]<=Context->tr2[i]) {
                  if (Context->south == south) {
                     if ((int)Context->tr1[i]>jmin) jmin=(int)Context->tr1[i];
                  } else {
                     if ((int)Context->tr2[i]<j) j=(int)Context->tr2[i];
                  }
               } else {
                  if (midcolor == north) {
                     for (l=jmin; l<=j && l<= (int)Context->tr2[i]; l++)
                         DarkenPixel(Context, i, l, north);
                     for (l=j; l>=jmin && l>= (int)Context->tr1[i]; l--)
                         DarkenPixel(Context, i, l, north);
                     break;
                  }
               }
               for (l=jmin; l<=j; l++) DarkenPixel(Context, i, l, north);
               break;
            }
         }

         Context->tr1[i] = tr1;
         Context->tr2[i] = tr2;
      }

      Context->south = south;
}
