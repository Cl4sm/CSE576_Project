void
quantize(Context)
Sundata * Context;
{
     int i, j, k, l, compare, quantum, change;
     unsigned short r[256], g[256], b[256];
     int count[256], done[256];
     char substit[256], value[256], inverse[256], compose[256];
     int d[COMPARE], v1[COMPARE], v2[COMPARE];
     int size, dist;
     XColor xc;

     if (verbose)
        fprintf(stderr, "Number of distinct colors in the map: %d colors\n",
                Context->ncolors);

     xc.flags = DoRed | DoGreen | DoBlue;

     for (i=0; i<256; i++) inverse[i] = '\0';

     for (i=0; i<Context->ncolors; i++) {
         count[i] = 0;
         xc.pixel = Context->daypixel[i];
         XQueryColor(dpy, tmp_cmap, &xc);
         r[i] = xc.red; g[i] = xc.green; b[i] = xc.blue;
	 inverse[(unsigned char)xc.pixel] = i;
     }

     if (tmp_cmap != cmap0)
        XFreeColormap(dpy, tmp_cmap);
     
     size = Context->xim->bytes_per_line * Context->xim->height;

     for (i=0; i<Context->ncolors; i++) {
         substit[i] = (char)i;
         value[i] = (char)i;
     }

     createGData(Context, 0);
     quantum = (256-Context->gdata->usedcolors)/2;     

     if (Context->ncolors<=quantum) goto finish;

     if (verbose)
        fprintf(stderr, "That's too much, quantizing to %d colors...\n", 
                 quantum);

     for (i=0; i<size; i++) ++count[(unsigned char)Context->xim->data[i]];
     
     compare = COMPARE;
     for (i=0; i<compare; i++) d[i] = 2147483647;

     for(i=0; i<Context->ncolors; i++)
        for(j=i+1; j<Context->ncolors; j++) {
            dist = abs((int)(r[i]-r[j]))+abs((int)(g[i]-g[j]))+
                                        +abs((int)(b[i]-b[j]));
            k=compare-1;
            while (k>=0 && dist<d[k]) k--;
            ++k;
            if (k<compare) {
              for (l=compare-1; l>k; l--) {
                  d[l] = d[l-1];
                  v1[l] = v1[l-1];
                  v2[l] = v2[l-1];
              }
              d[k] = dist;
              if (count[i]>count[j] || (count[i]==count[j] && i<j)) {
                 v1[k] = i;
                 v2[k] = j;
              } else {
                 v1[k] = j;
                 v2[k] = i;
              }
            }
        }

     l = 0;
     for (i=0; i<compare; i++) {
         j = v1[i];
         k = v2[i];
         if (substit[k]==(char) k) {
           substit[k] = j;
           ++l;
         }
         if (l>=Context->ncolors-quantum) break;
     }
     if (verbose)
        fprintf(stderr, 
           "%d substitutions from %d pairs of similar colors\n", l, i);

     change = 1;
     while (change) {
        change = 0;
        l = 0;
        for (i=0; i<Context->ncolors; i++) {
           j = (unsigned char) substit[i];
           if (substit[i]==(char)i) l++;
           if (substit[j] != (char)j) {
              substit[i] = substit[j];
              change = 1;
           }
        }
     }

  finish:

     if (verbose) {
        if (Context->gdata->cmap==cmap0)
           fprintf(stderr, "Allocating map colors in default colormap:\n");
        else         
           fprintf(stderr, "Allocating map colors in private colormap:\n");
     }
        
     for (i=0; i<Context->ncolors; i++) done[i] = 0;
     for (i=0; i<256; i++) Context->nightpixel[i] = (unsigned char)i;

     k=0;
     for (i=0; i<Context->ncolors; i++) {
        j = (unsigned char)substit[i];
        if (!done[j]) {
           xc.red = r[j];
           xc.green = g[j];
           xc.blue = b[j];
           if (!XAllocColor(dpy, Context->gdata->cmap, &xc)) {
              color_alloc_failed = 1;
              value[j] = 0;
           } else
              value[j] = (char)xc.pixel;
           xc.red = (unsigned int) (xc.red * Context->flags.darkness) / 255;
           xc.green = (unsigned int) (xc.green * Context->flags.darkness) / 255;
           xc.blue = (unsigned int) (xc.blue * Context->flags.darkness) / 255;
           if (!XAllocColor(dpy, Context->gdata->cmap, &xc)) 
              color_alloc_failed = 1;
           if (value[j]) {
	      Context->daypixel[k] = value[j];
              Context->nightpixel[(unsigned char)value[j]] = (char)xc.pixel;
	   }
           done[j] = 1;
           k++;
        }
     }

     if (Context->gdata->cmap==cmap0 && color_alloc_failed) {
        if (verbose) fprintf(stderr, "Failed !!\n");
        if (Context->gdata->links==0) free(Context->gdata);
        createGData(Context, 1);
        goto finish;
     }

     Context->ncolors = k;

     if (verbose)
        fprintf(stderr, "  2*%d+%d=%d colors allocated in colormap\n", 
	    k, Context->gdata->usedcolors, 2*k+Context->gdata->usedcolors);

     for (i=0; i<256; i++) compose[i] = 
       value[(unsigned char)substit[(unsigned char)inverse[(unsigned char)i]]];

     for (i=0; i<size; i++)
       Context->xim->data[i] = compose[(unsigned char)Context->xim->data[i]];
}
