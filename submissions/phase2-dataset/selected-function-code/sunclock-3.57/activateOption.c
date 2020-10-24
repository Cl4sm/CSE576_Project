activateOption()
{
        Sundata *Context;
	Flags oldflags;
        ZoomSettings oldzoom;
        int i, size;
	short *ptr, *oldptr, *newptr;
	double *zptr, *zoldptr, *znewptr;

	Context = OptionCaller;

	if (!do_option || !Context) return;

	oldflags = gflags;
	gflags.animate += 2;
	oldzoom = gzoom;
	runlevel = RUNTIMEOPTION;
	i = parseCmdLine(option_entry.string);
	correctValues();
        if (i>0 || runlevel == FAILEDOPTION) {
	   option_newhint = '?';
	   showOptionHint(-1);
	   return;
	} else
	   option_newhint = '\n';

        showOptionHint(-1);
     /* Set runlevel=IMAGERECYCLE if previous image/pixmap can be recycled */
	if (option_changes<4 && gflags.colorlevel==oldflags.colorlevel && 
            gflags.fillmode==oldflags.fillmode) {
           runlevel = IMAGERECYCLE;
	   tmp_cmap = Context->gdata->cmap;
	   if (gflags.colorlevel<FULLCOLORS) {
              clearNightArea(Context);
	      if (gflags.colorlevel==MONOCHROME) {
	         drawCities(Context);
	         drawSunAndMoon(Context);
	      }
	   } else {
  	      size = Context->xim->bytes_per_line*Context->xim->height;
              memcpy(Context->xim->data, Context->ximdata, size);
	   }
	}
	shutDown(Context, 0);
	memcpy(Context->spotsizes, city_spotsizes, city_cat*sizeof(int));
	memcpy(Context->sizelimits, city_sizelimits, city_cat*sizeof(int));

	if (gflags.animate<2) 
	   Context->flags.animate = gflags.animate;
	else
           gflags.animate -= 2;
	ptr = (short *) &gflags;
	oldptr = (short *) &oldflags;
	newptr = (short *) &Context->flags;
        for (i=0; i<sizeof(Flags)/sizeof(short); i++) 
            if (ptr[i]!=oldptr[i]) newptr[i] = ptr[i];

	zptr = (double *) &gzoom;
	zoldptr = (double *) &oldzoom;
	znewptr = (double *) &Context->zoom;
        for (i=0; i<6; i++) 
            if (zptr[i]!=zoldptr[i]) znewptr[i] = zptr[i];

	if (option_changes & 8)
	    Context->geom = ClockGeom;
	if (option_changes & 16)
	    Context->geom = MapGeom;
	if (option_changes & 32)
            StringReAlloc(&Context->clock_img_file, Clock_img_file);
	if (option_changes & 64)
            StringReAlloc(&Context->map_img_file, Map_img_file);
	buildMap(Context, Context->wintype, 0);
}
