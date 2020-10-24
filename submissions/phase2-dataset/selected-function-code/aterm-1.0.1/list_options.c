void
list_options()
{
#define INDENT 30
    fprintf(stderr, "(");
#ifdef BACKGROUND_IMAGE
    fprintf(stderr, "background image,");
#endif
#ifdef XPM
    fprintf(stderr, "XPM,");
#endif
#ifdef USE_LIBASIMAGE
#ifdef JPEG
    fprintf(stderr, "JPEG,");
#endif
#ifdef PNG
    fprintf(stderr, "PNG,");
#endif
    fprintf(stderr, "AfterStep integration,");
#ifdef _MYSTYLE_
    fprintf(stderr, "AfterStep MyStyles,");
#else
    fprintf(stderr, "no AfterStep MyStyles,");
#endif
#endif
#ifdef UTMP_SUPPORT
    fprintf(stderr, "utmp,");
#endif
#ifdef MENUBAR
    fprintf(stderr, "menubar,");
#endif
#ifdef KANJI
    fprintf(stderr, "Kanji,");
#endif
#ifdef ZH
    fprintf(stderr, "Chinese,");
#endif
#ifdef THAI
    fprintf(stderr, "Thai,");
#endif
#ifdef XTERM_SCROLLBAR
    fprintf(stderr, "XTerm-scrollbar,");
#endif
#ifdef GREEK_SUPPORT
    fprintf(stderr, "Greek,");
#endif
#ifdef NO_BACKSPACE_KEY
    fprintf(stderr, "no backspace,");
#endif
#ifdef NO_DELETE_KEY
    fprintf(stderr, "no delete,");
#endif
#ifdef TRANSPARENT
    fprintf(stderr, "transparency,");
#else
    fprintf(stderr, "no transparency,");
# endif
#ifdef OFF_FOCUS_FADING
    fprintf(stderr, "fading,");
#else
    fprintf(stderr, "no fading,");
# endif
#ifdef NEXT_SCROLLBAR
    fprintf(stderr, "NeXT scrollbar,");
# endif
#ifdef NO_RESOURCES
    fprintf(stderr, "NoResources");
#else
# ifdef USE_XGETDEFAULT
    fprintf(stderr, "XGetDefaults");
# else
    fprintf(stderr, ".Xdefaults");
# endif
#endif

    fprintf(stderr, ")" );

}
