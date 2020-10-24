{
	TextData	*data;

	data = XtNew(TextData);

	XtGetApplicationResources(selFileForm, (XtPointer) data, textResources,
		XtNumber(textResources), (Arg *) NULL, 0);

	SFfont = XLoadQueryFont(SFdisplay, data->fontname);
	if (!SFfont) {
		SFfont = XLoadQueryFont(SFdisplay, SF_DEFAULT_FONT);
		if (!SFfont) {
			char	sbuf[256];

			(void) sprintf(sbuf, "XsraSelFile: can't get font %s",
				SF_DEFAULT_FONT);

			XtAppError(SFapp, sbuf);
		}
	}

	SFcharWidth = (SFfont->max_bounds.width + SFfont->min_bounds.width) / 2;
	SFcharAscent = SFfont->max_bounds.ascent;
	SFcharHeight = SFcharAscent + SFfont->max_bounds.descent;
}
