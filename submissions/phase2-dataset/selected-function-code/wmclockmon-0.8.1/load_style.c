static void load_style(const char *stylename) {
    StyleDef  parts_style_opts[] = {
        {"Parts",          T_FILE, &style.parts,   &default_style.parts},
        {"BDigitHeight",   T_INT,  &style.bdigith, &default_style.bdigith},
        {"BDigitWidth",    T_INT,  &style.bdigitw, &default_style.bdigitw},
        {"SDigitHeight",   T_INT,  &style.sdigith, &default_style.sdigith},
        {"SDigitWidth",    T_INT,  &style.sdigitw, &default_style.sdigitw},
        {"BDigitSep",      T_INT,  &style.bsep,    &default_style.bsep},
        {"SDigitSep",      T_INT,  &style.ssep,    &default_style.ssep},
        {NULL,             T_STRING, NULL}
    };
    StyleDef  letters_style_opts[] = {
        {"Letters",        T_FILE, &style.letters, &default_style.letters},
        {"LetterHeight",   T_INT,  &style.letterh, &default_style.letterh},
        {"LetterWidth",    T_INT,  &style.letterw, &default_style.letterw},
        {"LetterSep",      T_INT,  &style.lsep,    &default_style.lsep},
        {NULL,             T_STRING, NULL}
    };
    StyleDef  itime_style_opts[] = {
        {"IBacklightOn",   T_FILE, &style.ibackl, &default_style.ibackl},
        {"IBacklightOff",  T_FILE, &style.ibacku, &default_style.ibacku},
        {"Beats_PosX",     T_INT,  &style.bposx,  &default_style.bposx},
        {"Beats_PosY",     T_INT,  &style.bposy,  &default_style.bposy},
        {"Beats_Big",      T_BOOL, &style.bbig,   &default_style.bbig},
        {"10thOB_PosX",    T_INT,  &style.tposx,  &default_style.tposx},
        {"10thOB_PosY",    T_INT,  &style.tposy,  &default_style.tposy},
        {"10thOB_Big",     T_BOOL, &style.tbig,   &default_style.tbig},
        {"10thOB_Display", T_BOOL, &style.tdisp,  &default_style.tdisp},
        {"Graph_PosX",     T_INT,  &style.gposx,  &default_style.gposx},
        {"Graph_PosY",     T_INT,  &style.gposy,  &default_style.gposy},
        {"Graph_Display",  T_BOOL, &style.gdisp,  &default_style.gdisp},
        {NULL,             T_STRING, NULL}
    };
    StyleDef  btime_style_opts[] = {
        {"BBacklightOn",  T_FILE, &style.bbackl, &default_style.bbackl},
        {"BBacklightOff", T_FILE, &style.bbacku, &default_style.bbacku},
        {"Bin_HX",        T_INT,  &style.binhx,  &default_style.binhx},
        {"Bin_HY",        T_INT,  &style.binhy,  &default_style.binhy},
        {"Bin_MX",        T_INT,  &style.binmx,  &default_style.binmx},
        {"Bin_MY",        T_INT,  &style.binmy,  &default_style.binmy},
        {"Bin_SX",        T_INT,  &style.binsx,  &default_style.binsx},
        {"Bin_SY",        T_INT,  &style.binsy,  &default_style.binsy},
        {"Bin_ZX",        T_INT,  &style.binzx,  &default_style.binzx},
        {"Bin_ZY",        T_INT,  &style.binzy,  &default_style.binzy},
        {"Bin_WX",        T_INT,  &style.binwx,  &default_style.binwx},
        {"Bin_WY",        T_INT,  &style.binwy,  &default_style.binwy},
        {"Bin_DX",        T_INT,  &style.bindx,  &default_style.bindx},
        {"Bin_DY",        T_INT,  &style.bindy,  &default_style.bindy},
        {"Bin_OX",        T_INT,  &style.binox,  &default_style.binox},
        {"Bin_OY",        T_INT,  &style.binoy,  &default_style.binoy},
        {"Bin_IX",        T_INT,  &style.binix,  &default_style.binix},
        {"Bin_IY",        T_INT,  &style.biniy,  &default_style.biniy},
        {"Bin_d1X",       T_INT,  &style.bind1x, &default_style.bind1x},
        {"Bin_d1Y",       T_INT,  &style.bind1y, &default_style.bind1y},
        {"Bin_d2X",       T_INT,  &style.bind2x, &default_style.bind2x},
        {"Bin_d2Y",       T_INT,  &style.bind2y, &default_style.bind2y},
        {"Bin_d3X",       T_INT,  &style.bind3x, &default_style.bind3x},
        {"Bin_d3Y",       T_INT,  &style.bind3y, &default_style.bind3y},
        {"Bin_d4X",       T_INT,  &style.bind4x, &default_style.bind4x},
        {"Bin_d4Y",       T_INT,  &style.bind4y, &default_style.bind4y},
        {NULL,            T_STRING, NULL}
    };
    StyleDef  main_style_opts[] = {
        {"PartsStyle",     T_FILE, &style.parts_s, &default_style.parts_s},
        {"LettersStyle",   T_FILE, &style.letters_s, &default_style.letters_s},
        {"ITimeStyle",     T_FILE, &style.itime_s, &default_style.itime_s},
        {"BTimeStyle",     T_FILE, &style.btime_s, &default_style.btime_s},
        {"BacklightOn",    T_FILE, &style.backl, &default_style.backl},
        {"BacklightOff",   T_FILE, &style.backu, &default_style.backu},
        {"NbColors",       T_INT,  &style.ncolors, &default_style.ncolors},
        {"Hours_PosX",     T_INT,  &style.hposx, &default_style.hposx},
        {"Hours_PosY",     T_INT,  &style.hposy, &default_style.hposy},
        {"Hours_Big",      T_BOOL, &style.hbig,  &default_style.hbig},
        {"Minutes_PosX",   T_INT,  &style.mposx, &default_style.mposx},
        {"Minutes_PosY",   T_INT,  &style.mposy, &default_style.mposy},
        {"Minutes_Big",    T_BOOL, &style.mbig,  &default_style.mbig},
        {"Seconds_PosX",   T_INT,  &style.sposx, &default_style.sposx},
        {"Seconds_PosY",   T_INT,  &style.sposy, &default_style.sposy},
        {"Seconds_Big",    T_BOOL, &style.sbig,  &default_style.sbig},
        {"Seconds_Colon",  T_BOOL, &style.csec,  &default_style.csec},
        {"AM_PosX",        T_INT,  &style.aposx, &default_style.aposx},
        {"AM_PosY",        T_INT,  &style.aposy, &default_style.aposy},
        {"PM_PosX",        T_INT,  &style.pposx, &default_style.pposx},
        {"PM_PosY",        T_INT,  &style.pposy, &default_style.pposy},
        {"ALRM_PosX",      T_INT,  &style.lposx, &default_style.lposx},
        {"ALRM_PosY",      T_INT,  &style.lposy, &default_style.lposy},
        {"Weekday_PosX",   T_INT,  &style.wposx, &default_style.wposx},
        {"Weekday_PosY",   T_INT,  &style.wposy, &default_style.wposy},
        {"Day_PosX",       T_INT,  &style.dposx, &default_style.dposx},
        {"Day_PosY",       T_INT,  &style.dposy, &default_style.dposy},
        {"Month_PosX",     T_INT,  &style.oposx, &default_style.oposx},
        {"Month_PosY",     T_INT,  &style.oposy, &default_style.oposy},
        {NULL,             T_STRING, NULL}
    };
    char *partsfile   = xstrdup(style.parts_s);
    char *lettersfile = xstrdup(style.letters_s);
    char *itimefile   = xstrdup(style.itime_s);
    char *btimefile   = xstrdup(style.btime_s);
    char *filename;

    if (! stylename) return;
    filename = set_filename(stylename, ".mwcs");

    /* main style */
    load_stylepart(filename, main_style_opts);

    /* parts */
    if (stringdiff(partsfile, style.parts_s) == True)
        load_stylepart(style.parts_s, parts_style_opts);

    /* letters */
    if (stringdiff(lettersfile, style.letters_s) == True)
        load_stylepart(style.letters_s, letters_style_opts);

    /* internet time */
    if (stringdiff(itimefile, style.itime_s) == True)
        load_stylepart(style.itime_s, itime_style_opts);

    /* binary clock */
    if (stringdiff(btimefile, style.btime_s) == True)
        load_stylepart(style.btime_s, btime_style_opts);

    FREE(partsfile);
    FREE(lettersfile);
    FREE(itimefile);
    FREE(btimefile);
    FREE(filename);
}