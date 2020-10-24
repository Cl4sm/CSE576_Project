static
int error(Tcl_Interp *interp, const char *msg)
{
  const char *str;
  char c[2];

  str  = strerror(errno);
  c[0] = tolower(*str);
  c[1] = 0;

  Tcl_ResetResult(interp);

  if (msg)
    Tcl_AppendResult(interp, msg, ": ", (char *) 0);

  if (hfs_error == 0)
    Tcl_AppendResult(interp, c, str + 1, (char *) 0);
  else
    Tcl_AppendResult(interp, hfs_error, " (", str, ")", (char *) 0);

  return TCL_ERROR;
}
