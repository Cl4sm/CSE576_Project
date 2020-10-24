void print(frontend *fe)
{
#ifndef _WIN32_WCE
    PRINTDLG pd;
    char doctitle[256];
    document *doc;
    midend *nme = NULL;  /* non-interactive midend for bulk puzzle generation */
    int i;
    char *err = NULL;

    /*
     * Create our document structure and fill it up with puzzles.
     */
    doc = document_new(fe->printw, fe->printh, fe->printscale / 100.0F);
    for (i = 0; i < fe->printcount; i++) {
	if (i == 0 && fe->printcurr) {
	    err = midend_print_puzzle(fe->me, doc, fe->printsolns);
	} else {
	    if (!nme) {
		game_params *params;

		nme = midend_new(NULL, fe->game, NULL, NULL);

		/*
		 * Set the non-interactive mid-end to have the same
		 * parameters as the standard one.
		 */
		params = midend_get_params(fe->me);
		midend_set_params(nme, params);
		fe->game->free_params(params);
	    }

	    midend_new_game(nme);
	    err = midend_print_puzzle(nme, doc, fe->printsolns);
	}
	if (err)
	    break;
    }
    if (nme)
	midend_free(nme);

    if (err) {
	MessageBox(fe->hwnd, err, "Error preparing puzzles for printing",
		   MB_ICONERROR | MB_OK);
	document_free(doc);
	return;
    }

    memset(&pd, 0, sizeof(pd));
    pd.lStructSize = sizeof(pd);
    pd.hwndOwner = fe->hwnd;
    pd.hDevMode = NULL;
    pd.hDevNames = NULL;
    pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC |
	PD_NOPAGENUMS | PD_NOSELECTION;
    pd.nCopies = 1;
    pd.nFromPage = pd.nToPage = 0xFFFF;
    pd.nMinPage = pd.nMaxPage = 1;

    if (!PrintDlg(&pd)) {
	document_free(doc);
	return;
    }

    /*
     * Now pd.hDC is a device context for the printer.
     */

    /*
     * FIXME: IWBNI we put up an Abort box here.
     */

    memset(&fe->di, 0, sizeof(fe->di));
    fe->di.cbSize = sizeof(fe->di);
    sprintf(doctitle, "Printed puzzles from %s (from Simon Tatham's"
	    " Portable Puzzle Collection)", fe->game->name);
    fe->di.lpszDocName = doctitle;
    fe->di.lpszOutput = NULL;
    fe->di.lpszDatatype = NULL;
    fe->di.fwType = 0;

    fe->drawstatus = PRINTING;
    fe->hdc = pd.hDC;

    fe->dr = drawing_new(&win_drawing, NULL, fe);
    document_print(doc, fe->dr);
    drawing_free(fe->dr);
    fe->dr = NULL;

    fe->drawstatus = NOTHING;

    DeleteDC(pd.hDC);
    document_free(doc);
#endif
}