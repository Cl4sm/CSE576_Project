void
DarkenPixel(Context, x, y, t)
struct Sundata * Context;
int x;
int y;
int t;
{
        register int i;
        unsigned int factor;
        unsigned char u, v, w, r, g, b;

        if (Context->flags.colorlevel<FULLCOLORS) {
           XDrawPoint(dpy, Context->mappix,Context->gdata->pixgc, x,y);
           return;
        }
        i = bytes_per_pixel * x + Context->xim->bytes_per_line * y;

        if (color_depth>16) {
           if (bigendian)
              i += bytes_per_pixel - 3;
           u = Context->ximdata[i];
           v = Context->ximdata[i+1];
           w = Context->ximdata[i+2];
           if (t>=0) {
              factor = Context->flags.darkness + (t * (MAXSHORT-
                  Context->flags.darkness))/Context->flags.colorscale;
              u = (u * factor)/MAXSHORT;
              v = (v * factor)/MAXSHORT;
              w = (w * factor)/MAXSHORT;
           }
           Context->xim->data[i] = u;
           Context->xim->data[i+1] = v;
           Context->xim->data[i+2] = w;
        } else 
        if (color_depth==16) {
	   if (bigendian) {
              u = Context->ximdata[i+1];
              v = Context->ximdata[i];
           } else {
              u = Context->ximdata[i];
              v = Context->ximdata[i+1];
           }
           if (t>=0) {
              factor = Context->flags.darkness + (t * (255 -
                 Context->flags.darkness))/Context->flags.colorscale;
              r = v>>3;
              g = ((v&7)<<3) | (u>>5);
              b = u&31;
              r = (r * factor)/31;
              g = (g * factor)/63;
              b = (b * factor)/31;
              u = (b&248)>>3 | (g&28)<<3;
              v = (g&224)>>5 | (r&248);
           }
           if (bigendian) {
              Context->xim->data[i+1] = u;
              Context->xim->data[i] = v;
           } else {
              Context->xim->data[i] = u;
              Context->xim->data[i+1] = v;
	   }
        } else
        if (color_depth==15) {
	   if (bigendian) {
              u = Context->ximdata[i+1];
              v = Context->ximdata[i];
	   } else {
              u = Context->ximdata[i];
              v = Context->ximdata[i+1];
	   }
           if (t>=0) {
              factor = Context->flags.darkness + (t * (255 - 
                 Context->flags.darkness))/Context->flags.colorscale;
              r = v>>2;
              g = (v&3)<<3 | (u>>5);
              b = u&31;
              r = (r * factor)/31;
              g = (g * factor)/31;
              b = (b * factor)/31;
              u = (b&248)>>3 | (g&56)<<2;
              v = (g&192)>>6 | (r&248)>>1;
           }
           if (bigendian) {
              Context->xim->data[i+1] = u;
              Context->xim->data[i] = v;
	   } else {
              Context->xim->data[i] = u;
              Context->xim->data[i+1] = v;
	   }
        } else {
           if (t>=0) {
             if ((277*x+359*y) % Context->flags.colorscale < 
                 Context->flags.colorscale-t)
               Context->xim->data[i] = 
                 Context->nightpixel[(unsigned char)Context->ximdata[i]];
             else
               Context->xim->data[i] = Context->ximdata[i];
           } else
             Context->xim->data[i] = Context->ximdata[i];
        }
}
