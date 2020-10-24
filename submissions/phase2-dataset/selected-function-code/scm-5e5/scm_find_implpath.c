     const char *execpath;
{
  char *implpath = 0;
#ifndef nosve
# ifndef POCKETCONSOLE
  char *getenvpath = getenv(INIT_GETENV);
  /* fprintf(stderr, "%s=%s\n", INIT_GETENV, getenvpath); fflush(stderr); */
  if (getenvpath) implpath = scm_cat_path(0L, getenvpath, 0L);
  if (implpath) {/* The value of the environment variable supersedes
		    other locations, only if the file exists. */
    implpath = scm_try_path(implpath);
    if (!implpath) {
      fputs("Value of "INIT_GETENV" (=\"", stderr);
      fputs(getenvpath, stderr);
      fputs("\") not found; Trying elsewhere\n", stderr);
    }
  }
# endif
#endif
  if (!implpath && execpath)
    implpath = find_impl_file(execpath, GENERIC_NAME, INIT_FILE_NAME, dirsep);
#ifdef IMPLINIT
  if (!implpath) implpath = scm_cat_path(0L, IMPLINIT, 0L);
#endif
  return implpath;
}
