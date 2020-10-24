static
int do_copynative(Tcl_Interp *interp, hfsfile *ifile, hfsfile *ofile)
{
  int result;

  if (hfs_setfork(ifile, 0) == -1 ||
      hfs_setfork(ofile, 0) == -1)
    return error(interp, "error opening data fork");

  result = fork_native(interp, ifile, ofile);
  if (result != TCL_OK)
    return result;

  if (hfs_setfork(ifile, 1) == -1 ||
      hfs_setfork(ofile, 1) == -1)
    return error(interp, "error opening resource fork");

  result = fork_native(interp, ifile, ofile);
  if (result != TCL_OK)
    return result;

  return TCL_OK;
}
