void gtk_draw_text(void *handle, int x, int y, int fonttype, int fontsize,
		   int align, int colour, char *text)
{
    frontend *fe = (frontend *)handle;
    int i;

    /*
     * Find or create the font.
     */
    for (i = 0; i < fe->nfonts; i++)
        if (fe->fonts[i].type == fonttype && fe->fonts[i].size == fontsize)
            break;

    if (i == fe->nfonts) {
        if (fe->fontsize <= fe->nfonts) {
            fe->fontsize = fe->nfonts + 10;
            fe->fonts = sresize(fe->fonts, fe->fontsize, struct font);
        }

        fe->nfonts++;

        fe->fonts[i].type = fonttype;
        fe->fonts[i].size = fontsize;
	add_font(fe, i, fonttype, fontsize);
    }

    /*
     * Do the job.
     */
    set_colour(fe, colour);
    align_and_draw_text(fe, i, align, x, y, text);
}