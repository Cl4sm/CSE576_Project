static void ps_begin_doc(void *handle, int pages)
{
    psdata *ps = (psdata *)handle;

    fputs("%!PS-Adobe-3.0\n", ps->fp);
    fputs("%%Creator: Simon Tatham's Portable Puzzle Collection\n", ps->fp);
    fputs("%%DocumentData: Clean7Bit\n", ps->fp);
    fputs("%%LanguageLevel: 1\n", ps->fp);
    fprintf(ps->fp, "%%%%Pages: %d\n", pages);
    fputs("%%DocumentNeededResources:\n", ps->fp);
    fputs("%%+ font Helvetica\n", ps->fp);
    fputs("%%+ font Courier\n", ps->fp);
    fputs("%%EndComments\n", ps->fp);
    fputs("%%BeginSetup\n", ps->fp);
    fputs("%%IncludeResource: font Helvetica\n", ps->fp);
    fputs("%%IncludeResource: font Courier\n", ps->fp);
    fputs("%%EndSetup\n", ps->fp);
    fputs("%%BeginProlog\n", ps->fp);
    /*
     * Re-encode Helvetica and Courier into ISO-8859-1, which gives
     * us times and divide signs - and also (according to the
     * Language Reference Manual) a bonus in that the ASCII '-' code
     * point now points to a minus sign instead of a hyphen.
     */
    fputs("/Helvetica findfont " /* get the font dictionary */
	  "dup maxlength dict dup begin " /* create and open a new dict */
	  "exch " /* move the original font to top of stack */
	  "{1 index /FID ne {def} {pop pop} ifelse} forall "
				       /* copy everything except FID */
	  "/Encoding ISOLatin1Encoding def "
			      /* set the thing we actually wanted to change */
	  "/FontName /Helvetica-L1 def " /* set a new font name */
	  "FontName end exch definefont" /* and define the font */
	  "\n", ps->fp);
    fputs("/Courier findfont " /* get the font dictionary */
	  "dup maxlength dict dup begin " /* create and open a new dict */
	  "exch " /* move the original font to top of stack */
	  "{1 index /FID ne {def} {pop pop} ifelse} forall "
				       /* copy everything except FID */
	  "/Encoding ISOLatin1Encoding def "
			      /* set the thing we actually wanted to change */
	  "/FontName /Courier-L1 def " /* set a new font name */
	  "FontName end exch definefont" /* and define the font */
	  "\n", ps->fp);
    fputs("%%EndProlog\n", ps->fp);
}