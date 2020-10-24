static SCM scm_lreadparen(tok_buf, port, flgs, name)
     SCM tok_buf;
     SCM port;
     int flgs;
     char *name;
{
  SCM lst, fst,
    tmp = scm_lreadpr(tok_buf, port, (0xC & flgs) | ((3 & flgs) ? 2 : 0));
  if (UNDEFINED==tmp) return EOL;
  if (i_dot==tmp) {
    fst = scm_lreadr(tok_buf, port, (0xC & flgs) | ((3 & flgs) ? 1 : 0));
  closeit:
    tmp = scm_lreadpr(tok_buf, port, 0);
    if (UNDEFINED != tmp) wta(UNDEFINED, "missing close paren", name);
    return fst;
  }
  fst = lst = cons(tmp, EOL);
  while (UNDEFINED !=
	 (tmp = scm_lreadpr(tok_buf, port, (0xC & flgs) | ((3 & flgs) ? 2 : 0)))) {
    if (EOF_VAL==tmp) wta(lst, s_eofin, s_list);
    if (i_dot==tmp) {
      CDR(lst) = scm_lreadr(tok_buf, port, (0xC & flgs) | ((3 & flgs) ? 1 : 0));
      goto closeit;
    }
    lst = (CDR(lst) = cons(tmp, EOL));
  }
  return fst;
}
