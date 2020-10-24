static void ps_draw_text(void *handle, int x, int y, int fonttype,
			 int fontsize, int align, int colour, char *text)
{
    psdata *ps = (psdata *)handle;

    y = ps->ytop - y;
    ps_setcolour(ps, colour);
    ps_printf(ps, "/%s findfont %d scalefont setfont\n",
	      fonttype == FONT_FIXED ? "Courier-L1" : "Helvetica-L1",
	      fontsize);
    if (align & ALIGN_VCENTRE) {
	ps_printf(ps, "newpath 0 0 moveto (X) true charpath flattenpath"
		  " pathbbox\n"
		  "3 -1 roll add 2 div %d exch sub %d exch moveto pop pop\n",
		  y, x);
    } else {
	ps_printf(ps, "%d %d moveto\n", x, y);
    }
    ps_printf(ps, "(");
    while (*text) {
	if (*text == '\\' || *text == '(' || *text == ')')
	    ps_printf(ps, "\\");
	ps_printf(ps, "%c", *text);
	text++;
    }
    ps_printf(ps, ") ");
    if (align & (ALIGN_HCENTRE | ALIGN_HRIGHT))
	ps_printf(ps, "dup stringwidth pop %sneg 0 rmoveto show\n",
		  (align & ALIGN_HCENTRE) ? "2 div " : "");
    else
	ps_printf(ps, "show\n");
}