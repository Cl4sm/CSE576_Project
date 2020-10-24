int scm_io_error(port, what)
     SCM port;
     const char *what;
{
#ifdef HAVE_PIPE
# ifdef EPIPE
  if (EPIPE==errno) {
    if (verbose > 2) {
      err_head("WARNING");
      lputs(";;", cur_errp);
      lputs(what, cur_errp);
      lputs(": closing pipe ", cur_errp);
      scm_iprin1(port, cur_errp, 1);
      scm_newline(cur_errp);
    }
    close_port(port);
    if (*loc_broken_pipe && NIMP(*loc_broken_pipe))
      apply(*loc_broken_pipe, port, listofnull);
    return 0;
  }
# endif
#endif
  if (SCM_INTERRUPTED(errno)) {
    errno = 0;
    return !0;
  }
  wta(port, what, "Input/Output");
  return 0;			/* squelch warning */
}
