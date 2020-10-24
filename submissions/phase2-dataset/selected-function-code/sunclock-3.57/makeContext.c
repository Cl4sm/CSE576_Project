makeContext(Context, build)
struct Sundata * Context;
int build;
{
        if (build) {
           Context->win = 0;
	   Context->xim = NULL;
           Context->ximdata = NULL;
	   Context->label = NULL;
	   Context->vmfpixels = NULL;
           Context->mappix = 0;
           if (Context->wintype)
              Context->geom = MapGeom;
           else
              Context->geom = ClockGeom;
           Context->spotsizes = (int *) salloc(city_cat * sizeof(int));
           Context->sizelimits = (int *) salloc(city_cat * sizeof(int));
	   memcpy(Context->spotsizes, city_spotsizes, city_cat*sizeof(int));
 	   memcpy(Context->sizelimits, city_sizelimits, city_cat*sizeof(int));
           Context->zoom = gzoom;
           Context->oldzoom = gzoom;
           Context->flags = gflags;
           Context->jump = time_jump;
           Context->clock_img_file = strdup(Clock_img_file);
           Context->map_img_file = strdup(Map_img_file);
           Context->mark1.city = NULL;
           Context->mark1.flags = 0;
           Context->pos1.tz = NULL;
           Context->mark2.city = NULL;
           Context->mark2.flags = 0;
	   Context->lastmarked = NULL;
           Context->pos2.tz = NULL;
           Context->tr1 = Context->tr2 = NULL;
           if (position.lat<=90.0) {
              Context->pos1 = position;
              Context->mark1.city = &Context->pos1;
           }
        }

        Context->newzoom = Context->zoom;
        setZoomDimension(Context);
        Context->zoom = Context->newzoom;

        Context->local_day = -1;
        Context->solar_day = -1;
	Context->sundec = 100.0;
	Context->sunlon = 0.0;
	Context->moondec = 100.0;
	Context->moonlon = 0.0;

        if (runlevel!= IMAGERECYCLE) {
           if (color_depth<=8) {
              Context->daypixel = (unsigned char *) salloc(256*sizeof(char));
              Context->nightpixel = (unsigned char *) salloc(256*sizeof(char));
           } else {
              Context->daypixel = NULL;
              Context->nightpixel = NULL;
           }
	}

        Context->bits = 0;
        Context->flags.update = 4;
        Context->time = 0L;
        Context->projtime = -1L;
        Context->roottime = -1L;
        Context->animtime = -1L;
        Context->daywave = (double *) salloc( 
              (2*Context->geom.height+Context->geom.width)*sizeof(double));
        Context->cosval = Context->daywave + Context->geom.width;
        Context->sinval = Context->cosval + Context->geom.height;

        initShading(Context);
}
