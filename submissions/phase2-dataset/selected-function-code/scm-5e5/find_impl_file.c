char *find_impl_file(exec_path, generic_name, initname, sep)
     const char *exec_path;
     const char *generic_name, *initname, *sep;
{
  char *sepptr = strrchr(exec_path, sep[0]);
  char *extptr = exec_path + strlen(exec_path);
  char *path = 0;

#ifdef _WIN32
  char exec_buf[MAX_PATH];
  HMODULE mod = GetModuleHandle(0); /* Returns module handle to current executable. */

  if (mod) {
    GetModuleFileName(mod, exec_buf, sizeof(exec_buf));
    exec_path = exec_buf;
  }
#endif

  /*fprintf(stderr, "dld_find_e %s\n", exec_path); fflush(stderr);*/

  sepptr = strrchr(exec_path, sep[0]);
  extptr = exec_path + strlen(exec_path);

  if (sepptr) {
    long sepind = sepptr - exec_path + 1L;

    /* In case exec_path is in the source directory, look first in
       exec_path's directory. */
    path = scm_cat_path(0L, exec_path, sepind - 1L);
    path = scm_sep_init_try(path, sep, initname);
    if (path) return path;

#ifdef MSDOS
    if (!strcmp(extptr - 4, ".exe") || !strcmp(extptr - 4, ".com") ||
	!strcmp(extptr - 4, ".EXE") || !strcmp(extptr - 4, ".COM"))
      extptr = extptr - 4;
#endif /* def MSDOS */

    if (generic_name &&
	!strncmp(exec_path + sepind, generic_name, extptr - exec_path))
      generic_name = 0;

    /* If exec_path is in directory "exe" or "bin": */
    path = scm_cat_path(0L, exec_path, sepind - 1L);
    sepptr = path + sepind - 4;
    if (!strcmp(sepptr, "exe") || !strcmp(sepptr, "bin") ||
	!strcmp(sepptr, "EXE") || !strcmp(sepptr, "BIN")) {
      char *peer;

      /* Look for initname in peer directory "lib". */
      if (path) {
	strncpy(sepptr, "lib", 3);
	path = scm_sep_init_try(path, sep, initname);
	if (path) return path;
      }

      /* Look for initname in peer directories "lib" and "src" in
	 subdirectory with the name of the executable (sans any type
	 extension like .EXE). */
      for (peer="lib";!0;peer="src") {
	path = scm_cat_path(0L, exec_path, extptr - exec_path + 0L);
	if (path) {
	  strncpy(path + sepind - 4, peer, 3);
	  path[extptr - exec_path] = 0;
	  path = scm_sep_init_try(path, sep, initname);
	  if (path) return path;
	}
	if (!strcmp(peer, "src")) break;
      }

      if (generic_name) {

	/* Look for initname in peer directories "lib" and "src" in
	   subdirectory with the generic name. */
	for (peer="lib";!0;peer="src") {
	  path = scm_cat_path(0L, exec_path, sepind);
	  if (path) {
	    strncpy(path + sepind - 4, peer, 3);
	    path = scm_cat_path(path, generic_name, 0L);
	    path = scm_sep_init_try(path, sep, initname);
	    if (path) return path;
	  }
	  if (!strcmp(peer, "src")) break;
	}}

      /* Look for initname in executable-name peer directory. */
      path = scm_cat_path(0L, exec_path, sepind);
      if (path) {
	path[sepind - 4] = 0;
	path = scm_cat_path(path, &exec_path[sepind], 0L);
	path = scm_sep_init_try(path, sep, initname);
	if (path) return path;
      }

      if (generic_name) {

	/* Look for initname in generic peer directory. */
	path = scm_cat_path(0L, exec_path, sepind);
	if (path) {
	  path[sepind - 4] = 0;
	  path = scm_cat_path(path, generic_name, 0L);
	  path = scm_sep_init_try(path, sep, initname);
	  if (path) return path;
	}
      }
    }

#ifdef MSDOS
    if (strlen(extptr)) {
      /* If exec_path has type extension, look in a subdirectory with
	 the name of the executable sans the executable file's type
	 extension. */
      path = scm_cat_path(0L, exec_path, extptr - exec_path + 0L);
      path = scm_sep_init_try(path, sep, initname);
      if (path) return path;

      if (generic_name) {

	/* Also look in generic_name subdirectory. */
	path = scm_cat_path(0L, exec_path, sepind);
	if (path) path = scm_cat_path(path, generic_name, 0L);
	path = scm_sep_init_try(path, sep, initname);
	if (path) return path;
      }}
#endif /* def MSDOS */
  }
  else {

    /* We don't have a parse-able exec_path.  The only path to try is
       just initname. */
    path = scm_cat_path(0L, initname, 0L);
    if (path) path = scm_try_path(path);
    if (path) return path;
  }
  return 0L;
}
