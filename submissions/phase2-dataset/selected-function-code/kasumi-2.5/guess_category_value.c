static const char *
internal_function
guess_category_value (category, categoryname)
     int category;
     const char *categoryname;
{
  const char *language;
  const char *retval;

  /* The highest priority value is the `LANGUAGE' environment
     variable.  But we don't use the value if the currently selected
     locale is the C locale.  This is a GNU extension.  */
  language = getenv ("LANGUAGE");
  if (language != NULL && language[0] == '\0')
    language = NULL;

  /* We have to proceed with the POSIX methods of looking to `LC_ALL',
     `LC_xxx', and `LANG'.  On some systems this can be done by the
     `setlocale' function itself.  */
#ifdef _LIBC
  retval = __current_locale_name (category);
#else
  retval = _nl_locale_name (category, categoryname);
#endif

  /* Ignore LANGUAGE if the locale is set to "C" because
     1. "C" locale usually uses the ASCII encoding, and most international
	messages use non-ASCII characters. These characters get displayed
	as question marks (if using glibc's iconv()) or as invalid 8-bit
	characters (because other iconv()s refuse to convert most non-ASCII
	characters to ASCII). In any case, the output is ugly.
     2. The precise output of some programs in the "C" locale is specified
	by POSIX and should not depend on environment variables like
	"LANGUAGE".  We allow such programs to use gettext().  */
  return language != NULL && strcmp (retval, "C") != 0 ? language : retval;
}
