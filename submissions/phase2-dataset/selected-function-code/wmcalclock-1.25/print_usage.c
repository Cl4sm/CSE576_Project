
    printf("\nwmCalClock version: %s\n", WMCALCLOCK_VERSION);
    printf("\nusage: wmCalClock [-b <Volume>] [-tc <Color>] [-bc <Color>] [-e \"Command\"] [-S]\n");
    printf("         [-24] [-g] [-s] [-l <longitude>] [-l] [-jazz] [-tekton] [-luggerbug]\n");
    printf("         [-arial] [-comicsans] [-h]\n\n");
    printf("\t-b <Volume>\tBeep on the hour. Volume is between -100 to 100.\n");
    printf("\t-tekton\t\tUse the Tekton font for time field.\n");
    printf("\t-arial\t\tUse the Arial-Narrow (i.e. Helvetica-Narrow) font for time field.\n");
    printf("\t-jazz\t\tUse the JazzPoster font for time field.\n");
    printf("\t-luggerbug\tUse the Luggerbug font for time field.\n");
    printf("\t-comicsans\tUse the ComicSans font for time field.\n");
    printf("\t-tc <Color>\tColor of the time digits (e.g. red or #ff8800).\n");
    printf("\t-bc <Color>\tBackground color.\n");
    printf("\t-e \"Command\"\tCommand to execute via double click of mouse button 1.\n");
    printf("\t-S\t\tDo not show seconds.\n");
    printf("\t-24\t\tShow 24-hour time. Default is 12 hour AM/PM Time.\n");
    printf("\t-g\t\tShow Greenwich time.\n");
    printf("\t-s\t\tShow Greenwich Mean Sidereal Time (GMST) in 24-hour format. \n");
    printf("\t-L <Longitude>\tShow Local Sidereal Time (LST) in 24-hour format. \n");
    printf("\t              \t\tLongitude is in degrees (- for West + for East).\n");
    printf("\t-l\t\tUse a low-color pixmap to conserve colors. On 8-bit displays the\n");
    printf("\t  \t\tlow color pixmap will always be used.\n");
    printf("\t-h\t\tDisplay help screen.\n");
    printf("\nExample: wmCalClock -b 100 -tc '#001100' -bc '#7e9e69' \n\n");

}
