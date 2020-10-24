int cmd_exit(ClientData clientData, Tcl_Interp *interp,
	     int argc, char *argv[])
{
  int status = 0;

  if (argc > 2)
    {
      interp->result = "wrong # args: should be \"exit ?returnCode?\"";
      return TCL_ERROR;
    }

  if (argc == 2 && Tcl_GetInt(interp, argv[1], &status) != TCL_OK)
    return TCL_ERROR;

  hfs_umountall();

  exit(status);

  return TCL_OK;
}
