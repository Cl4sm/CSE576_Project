#else
String PWGetFilepath(w, str)
    Widget w;
    String *str;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    String end;

    *str = XtNewString(PW->pixmap.filename);
    end = rindex(*str, '/');

    if (end)
	*(end + 1) = '\0';
    else 
	**str = '\0';

    return *str;
}
