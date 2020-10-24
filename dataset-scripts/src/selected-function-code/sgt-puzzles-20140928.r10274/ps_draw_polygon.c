static void ps_draw_polygon(void *handle, int *coords, int npoints,
			    int fillcolour, int outlinecolour)
{
    psdata *ps = (psdata *)handle;

    int i;

    ps_printf(ps, "newpath %d %d moveto\n", coords[0], ps->ytop - coords[1]);

    for (i = 1; i < npoints; i++)
	ps_printf(ps, "%d %d lineto\n", coords[i*2], ps->ytop - coords[i*2+1]);

    ps_printf(ps, "closepath\n");

    if (fillcolour >= 0) {
	ps_printf(ps, "gsave\n");
	ps_fill(ps, fillcolour);
	ps_printf(ps, "grestore\n");
    }
    ps_stroke(ps, outlinecolour);
}