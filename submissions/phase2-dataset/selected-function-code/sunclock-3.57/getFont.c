XFontStruct *
getFont(num)
int num;
{
XFontStruct * font;

    font = XLoadQueryFont(dpy, SunFont[num]);
    if (font == (XFontStruct *)NULL)
       fprintf(stderr, "%s: can't open font `%s', using `%s'\n",
                ProgName, SunFont[num], FAILFONT);
    else
       return font;
    font = XLoadQueryFont(dpy, FAILFONT);
    if (font == (XFontStruct *)NULL) {
       fprintf(stderr, "%s: can't open font `%s', giving up\n",
               ProgName, FAILFONT);
       return NULL;
    }
    return font;
}
