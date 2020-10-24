static void graphics_init() {
    XpmColorSymbol colors[3] = { {"Back0", NULL, 0}, {"Back1", NULL, 0}, {"Back2", NULL, 0} };
    int ncolor = 0;

    if (light_color) {
        ncolor = style.ncolors;
        if (ncolor == 2) {
            colors[0].pixel = dockapp_getcolor(light_color);
            colors[1].pixel = dockapp_blendedcolor(light_color, -24, -24, -24, 1.0);
        } else {
            colors[0].pixel = dockapp_getcolor(light_color);
            colors[1].pixel = dockapp_dividecolor(light_color, 3);
            colors[2].pixel = dockapp_blendedcolor(light_color, -50, -50, -50, 1.0);
        }
    }

    /* change raw xpm data to pixmap */
    /*
    if (dockapp_iswindowed)
        backlit_pix[1] = backgrd_pix[1] = WINDOWED_BG;
    */

    if (style.parts)
        init_pixfile(style.parts, &parts, "parts",
                colors, ncolor, False);
    else
        init_pixmap(parts_xpm,    &parts, "parts",
                colors, ncolor, False);

    if (style.letters)
        init_pixfile(style.letters, &letters, "letters",
                colors, ncolor, False);
    else
        init_pixmap(letters_xpm,    &letters, "letters",
                colors, ncolor, False);

    if (style.ibackl)
        init_pixfile(style.ibackl,     &backdropI_on, "backlit background 2",
                colors, ncolor, False);
    else
        init_pixmap(backlightI_on_xpm, &backdropI_on, "backlit background 2",
                colors, ncolor, False);

    if (style.ibacku)
        init_pixfile(style.ibacku,      &backdropI_off, "background 2",
                colors, ncolor, False);
    else
        init_pixmap(backlightI_off_xpm, &backdropI_off, "background 2",
                colors, ncolor, False);

    if (style.bbackl)
        init_pixfile(style.bbackl,      &backdropB_on, "backlit background 3",
                colors, ncolor, False);
    else
        init_pixmap(backlightB_on_xpm, &backdropB_on, "backlit background 3",
                colors, ncolor, False);

    if (style.bbacku)
        init_pixfile(style.bbacku,      &backdropB_off, "background 3",
                colors, ncolor, False);
    else
        init_pixmap(backlightB_off_xpm, &backdropB_off, "background 3",
                colors, ncolor, False);

    if (style.backl)
        init_pixfile(style.backl,      &backdrop_on, "backlit background",
                colors, ncolor, False);
    else
        init_pixmap(backlight0_on_xpm, &backdrop_on, "backlit background",
                colors, ncolor, False);

    if (style.backu)
        init_pixfile(style.backu,       &backdrop_off, "background",
                colors, ncolor, True);
    else
        init_pixmap(backlight0_off_xpm, &backdrop_off, "background",
                colors, ncolor, True);

    /* shape window */
    if (!dockapp_iswindowed) dockapp_setshape(mask, 0, 0);
    if (mask) XFreePixmap(display, mask);

    /* pixmap : draw area */
    if (pixmap) XFreePixmap(display, pixmap);
    pixmap = dockapp_XCreatePixmap(SIZE, SIZE);

    /* Initialize pixmap */
    draw_background(backlight);

    dockapp_set_background(pixmap);
    update();
    dockapp_show();
}