static void win_end_doc(void *handle)
{
    frontend *fe = (frontend *)handle;

    assert(fe->drawstatus != DRAWING);

    /*
     * Free all the fonts created since we began printing.
     */
    while (fe->nfonts > fe->fontstart) {
	fe->nfonts--;
	DeleteObject(fe->fonts[fe->nfonts].font);
    }
    fe->fontstart = 0;

    /*
     * The MSDN web site sample code doesn't bother to call EndDoc
     * if an error occurs half way through printing. I expect doing
     * so would cause the erroneous document to actually be
     * printed, or something equally undesirable.
     */
    if (fe->drawstatus == NOTHING)
	return;

    if (EndDoc(fe->hdc) <= 0) {
	char *e = geterrstr();
	MessageBox(fe->hwnd, e, "Error finishing printing",
		   MB_ICONERROR | MB_OK);
	sfree(e);
	fe->drawstatus = NOTHING;
    }
}