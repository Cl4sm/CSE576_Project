SCM tryload(filename, reader)
     SCM filename, reader;
{
  ASRTER(NIMP(filename) && STRINGP(filename), filename, ARG1, s_load);
  if (FALSEP(reader)) reader = UNDEFINED;
#ifndef RECKLESS
  if (!UNBNDP(reader)) scm_arity_check(reader, 1L, s_load);
#endif
  {
    SCM oloadpath = *loc_loadpath;
    SCM oloadports = loadports;
    SCM form, port;
    SCM env = EOL;
    port = open_file(filename, makfromstr("r?", (sizet)2*sizeof(char)));
    if (FALSEP(port)) return port;
    *loc_loadpath = filename;
    loadports = cons(port, loadports);
#ifdef SCM_ENV_FILENAME
    env = scm_env_addprop(SCM_ENV_FILENAME, filename, env);
#endif
    while(1) {
      if (UNBNDP(reader))
	form = scm_read_for_load(port);
      else
	form = scm_cvapply(reader, 1L, &port);
      if (EOF_VAL==form) break;
      SIDEVAL(form, env, EOL);
    }
    close_port(port);
    loadports = oloadports;
    *loc_loadpath = oloadpath;
  }
  return BOOL_T;
}
