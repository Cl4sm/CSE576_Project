void
scr_printscreen(int fullhist)
{
#ifdef PRINTPIPE
    int             i, r, nrows, row_offset;
    text_t         *t;
    FILE           *fd;

    if ((fd = popen_printer()) == NULL)
	return;
    nrows = TermWin.nrow;
    if (fullhist)
	nrows += TermWin.view_start;

    row_offset = TermWin.saveLines - TermWin.view_start;
    for (r = 0; r < nrows; r++) {
	t = screen.text[r + row_offset];
	for (i = TermWin.bcol - 1; i >= 0; i--)
	    if (!isspace(t[i]))
		break;
	fprintf(fd, "%.*s\n", (i + 1), t);
    }
    pclose_printer(fd);
#endif
}
