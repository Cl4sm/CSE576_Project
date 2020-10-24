static void win_draw_text(void *handle, int x, int y, int fonttype,
			  int fontsize, int align, int colour, char *text)
{
    frontend *fe = (frontend *)handle;
    POINT xy;
    int i;
    LOGFONT lf;

    if (fe->drawstatus == NOTHING)
	return;

    if (fe->drawstatus == PRINTING)
	fontsize = (int)(fontsize * fe->printpixelscale);

    xy = win_transform_point(fe, x, y);

    /*
     * Find or create the font.
     */
    for (i = fe->fontstart; i < fe->nfonts; i++)
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

        memset (&lf, 0, sizeof(LOGFONT));
        lf.lfHeight = -fontsize;
        lf.lfWeight = (fe->drawstatus == PRINTING ? 0 : FW_BOLD);
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
        lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        lf.lfQuality = DEFAULT_QUALITY;
        lf.lfPitchAndFamily = (fonttype == FONT_FIXED ?
                               FIXED_PITCH | FF_DONTCARE :
                               VARIABLE_PITCH | FF_SWISS);
#ifdef _WIN32_WCE
        wcscpy(lf.lfFaceName, TEXT("Tahoma"));
#endif

        fe->fonts[i].font = CreateFontIndirect(&lf);
    }

    /*
     * Position and draw the text.
     */
    {
	HFONT oldfont;
	TEXTMETRIC tm;
	SIZE size;
	TCHAR wText[256];
	MultiByteToWideChar (CP_UTF8, 0, text, -1, wText, 256);

	oldfont = SelectObject(fe->hdc, fe->fonts[i].font);
	if (GetTextMetrics(fe->hdc, &tm)) {
	    if (align & ALIGN_VCENTRE)
		xy.y -= (tm.tmAscent+tm.tmDescent)/2;
	    else
		xy.y -= tm.tmAscent;
	}
	if (GetTextExtentPoint32W(fe->hdc, wText, wcslen(wText), &size))
	{
	    if (align & ALIGN_HCENTRE)
		xy.x -= size.cx / 2;
	    else if (align & ALIGN_HRIGHT)
		xy.x -= size.cx;
	}
	SetBkMode(fe->hdc, TRANSPARENT);
	win_text_colour(fe, colour);
	ExtTextOutW(fe->hdc, xy.x, xy.y, 0, NULL, wText, wcslen(wText), NULL);
	SelectObject(fe->hdc, oldfont);
    }
}