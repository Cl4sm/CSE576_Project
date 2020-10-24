void
display_help_text (void)
{
    printf (_("Usage: %s [OPTION]... SOURCE DEST [[D:]X,Y[,S]|[,SX,SY]]...\n"
              "Interpolate pixels in JFIF image files.\n\n"
              "Pixel block specification:\n"
              "  D     can be `V' or `v' (vertical 1D interpolation),\n"
              "               `H' or `h' (horizontal 1D interpolation),\n"
              "               `2'        (2D interpolation) [default];\n"
              "  X,Y   specifies the top left corner of the pixel block to be interpolated;\n"
              "  S     specifies the size of the block [default: 1];\n"
              "  SX,SY specifies separate sizes for the X and Y direction.\n"
              "All numbers can be absolute coordinates/sizes, or percentages of the image\n"
              "size (if followed by a `%%' character).\n\n"
              "Options:\n"), invocation_name);
    fputs (STR_HELP, stdout);

    /* TRANSLATORS: Please align the right text column in the
       "Interpolation methods" section with the right text column in the
       "Options" section.  */
    fputs (_("\nInterpolation methods:\n"
             "  0, av, average            average of adjacent pixels\n"
             "  1, li, linear             (bi)linear interpolation\n"
             "  2, qu, quadratic          (bi)quadratic interpolation\n"
             "  3, cu, cubic              (bi)cubic interpolation\n"), stdout);

    /* TRANSLATORS: Please include the information that bug reports
       should be send in English.  E.g., translate "Please report bugs
       (in English) to ...".  */
    fputs (_("\nPlease report bugs to <martin@zero-based.org>.\n"), stdout);
}