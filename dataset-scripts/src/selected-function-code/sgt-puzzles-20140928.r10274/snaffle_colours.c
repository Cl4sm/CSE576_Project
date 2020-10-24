static void snaffle_colours(frontend *fe)
{
    int i, ncolours;
    float *colours;
    gboolean *success;

    fe->colmap = gdk_colormap_get_system();
    colours = midend_colours(fe->me, &ncolours);
    fe->ncolours = ncolours;
    fe->colours = snewn(ncolours, GdkColor);
    for (i = 0; i < ncolours; i++) {
	fe->colours[i].red = colours[i*3] * 0xFFFF;
	fe->colours[i].green = colours[i*3+1] * 0xFFFF;
	fe->colours[i].blue = colours[i*3+2] * 0xFFFF;
    }
    success = snewn(ncolours, gboolean);
    gdk_colormap_alloc_colors(fe->colmap, fe->colours, ncolours,
			      FALSE, FALSE, success);
    for (i = 0; i < ncolours; i++) {
	if (!success[i]) {
	    g_error("couldn't allocate colour %d (#%02x%02x%02x)\n",
		    i, fe->colours[i].red >> 8,
		    fe->colours[i].green >> 8,
		    fe->colours[i].blue >> 8);
	}
    }
}