static void ps_fill(psdata *ps, int colour)
{
    int hatch;
    float r, g, b;

    print_get_colour(ps->drawing, colour, ps->colour, &hatch, &r, &g, &b);

    if (hatch < 0) {
	if (ps->colour)
	    ps_printf(ps, "%g %g %g setrgbcolor fill\n", r, g, b);
	else
	    ps_printf(ps, "%g setgray fill\n", r);
    } else {
	/* Clip to the region. */
	ps_printf(ps, "gsave clip\n");
	/* Hatch the entire game printing area. */
	ps_printf(ps, "newpath\n");
	if (hatch == HATCH_VERT || hatch == HATCH_PLUS)
	    ps_printf(ps, "0 %g %d {\n"
		      "  0 moveto 0 %d rlineto\n"
		      "} for\n", ps->hatchspace, ps->gamewidth,
		      ps->gameheight);
	if (hatch == HATCH_HORIZ || hatch == HATCH_PLUS)
	    ps_printf(ps, "0 %g %d {\n"
		      "  0 exch moveto %d 0 rlineto\n"
		      "} for\n", ps->hatchspace, ps->gameheight,
		      ps->gamewidth);
	if (hatch == HATCH_SLASH || hatch == HATCH_X)
	    ps_printf(ps, "%d %g %d {\n"
		      "  0 moveto %d dup rlineto\n"
		      "} for\n", -ps->gameheight, ps->hatchspace * ROOT2,
		      ps->gamewidth, max(ps->gamewidth, ps->gameheight));
	if (hatch == HATCH_BACKSLASH || hatch == HATCH_X)
	    ps_printf(ps, "0 %g %d {\n"
		      "  0 moveto %d neg dup neg rlineto\n"
		      "} for\n", ps->hatchspace * ROOT2,
		      ps->gamewidth+ps->gameheight,
		      max(ps->gamewidth, ps->gameheight));
	ps_printf(ps, "0 setgray %g setlinewidth stroke grestore\n",
		  ps->hatchthick);
    }
}