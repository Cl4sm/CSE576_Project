SCM l_pipe()
{
  int fd[2], ret;
  FILE *f_rd, *f_wt;
  SCM p_rd, p_wt;
  NEWCELL(p_rd); NEWCELL(p_wt);
  DEFER_INTS;
  SYSCALL(ret = pipe(fd););
  if (ret) {ALLOW_INTS; return BOOL_F;}
  SYSCALL(f_rd = fdopen(fd[0], "r"););
  if (!f_rd) {
    close(fd[0]);
    goto errout;
  }
  SCM_OPENCALL(f_wt = fdopen(fd[1], "w"));
  if (!f_wt) {
    fclose(f_rd);
  errout:
    close(fd[1]);
    wta(UNDEFINED, (char *)NALLOC, s_port_type);
  }
  p_rd = scm_port_entry(f_rd, tc16_fport, mode_bits("r", (char *)0));
  p_wt = scm_port_entry(f_wt, tc16_fport, mode_bits("w", (char *)0));
  ALLOW_INTS;
  return cons(p_rd, p_wt);
}
