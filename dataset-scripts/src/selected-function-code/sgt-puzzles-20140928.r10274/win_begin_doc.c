static void win_begin_doc(void *handle, int pages)
{
    frontend *fe = (frontend *)handle;

    assert(fe->drawstatus != DRAWING);
    if (fe->drawstatus == NOTHING)
	return;

    if (StartDoc(fe->hdc, &fe->di) <= 0) {
	char *e = geterrstr();
	MessageBox(fe->hwnd, e, "Error starting to print",
		   MB_ICONERROR | MB_OK);
	sfree(e);
	fe->drawstatus = NOTHING;
    }

    /*
     * Push a marker on the font stack so that we won't use the
     * same fonts for printing and drawing. (This is because
     * drawing seems to look generally better in bold, but printing
     * is better not in bold.)
     */
    fe->fontstart = fe->nfonts;
}