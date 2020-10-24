void
destroyGCs(Context)
Sundata * Context;
{
int i; 
	 if (Context->gdata->links>0) {
            --Context->gdata->links;
	    return;
	 }

         XFreeGC(dpy, Context->gdata->wingc);
	 Context->gdata->wingc = 0;
         XFreeGC(dpy, Context->gdata->pixgc);
         Context->gdata->pixgc = 0;

         if (runlevel!=IMAGERECYCLE && Context->gdata->cmap!=cmap0)
	    XFreeColormap(dpy, Context->gdata->cmap);

	 for (i=0; i<NUMFONTS; i++)
             XFreeFont(dpy, Context->gdata->font[i]);
	
	 free(Context->gdata);
}
