void
createGData(Context, private)
struct Sundata * Context;
int private;
{
        GraphicData *           graphdata;             
        Sundata *               OtherContext;
        int                     prec, i, j;

        /* Try to use already defined GCs and Pixels in default colormap
           if already defined */

        if (!private && runlevel == RUNNING) {

	   graphdata = NULL;
  	   prec = -1;

           for (OtherContext = Seed; OtherContext; 
                OtherContext = OtherContext->next) {
              if (OtherContext != Context &&
                  OtherContext->flags.colorlevel == gflags.colorlevel &&
                  OtherContext->gdata->cmap == cmap0 && 
                  OtherContext->gdata->precedence > prec) {
	         graphdata = OtherContext->gdata;
	         prec = OtherContext->gdata->precedence;
              }
           } 

	   if (graphdata) {
              Context->gdata = graphdata;
              ++Context->gdata->links;
	      return;
	   }
	}

    newcmap:

        /* Otherwise, define new adhoc graphical data */
        Context->gdata = (GraphicData *)salloc(sizeof(GraphicData));
        Context->gdata->wingc = 0;
        Context->gdata->pixgc = 0;
        Context->gdata->links = 0;
	Context->gdata->precedence = precedence;
	++precedence;
        if (color_depth>8 || Context->flags.colorlevel==MONOCHROME ||
           (Context->flags.colorlevel>MONOCHROME && !private))
           Context->gdata->cmap = cmap0;
        else
           Context->gdata->cmap =
                XCreateColormap(dpy, Root, visual, AllocNone);

        color_alloc_failed = 0;

        total_colors = 0;
        for (i=0; i<NUMPIXELS; i++)
            Context->gdata->pixel[i] = getColor(Context, i);

        if (color_depth<=8 && color_alloc_failed &&
           Context->gdata->cmap==cmap0) {
              private = 1;
              fprintf(stderr, 
                 "Color allocation failed with default colormap.\n"
                 "Trying instead private colormap...\n");
              goto newcmap;
	}

        if (color_alloc_failed) 
           fprintf(stderr, "Color allocation failed !!!\n");
	else
	   if (verbose)
              fprintf(stderr, "Color allocation successful:\n");

        Context->gdata->usedcolors = total_colors;
        for (i=0; i<total_colors; i++) {
           for (j=0; j<i; j++) if (Context->gdata->pixel[i]==Context->gdata->pixel[j]) { 
              --Context->gdata->usedcolors; 
              break;
           }
        }
        if (verbose && !color_alloc_failed)
           fprintf(stderr, 
              "  %d basic colors allocated in %s colormap.\n",
              Context->gdata->usedcolors,
              (Context->gdata->cmap==cmap0)? "default":"private");
}
