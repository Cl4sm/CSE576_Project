void file_ref(Tcl_Interp *interp, volref *vref, fileref *fref, hfsfile *file)
{
  static int id = 0;
  Tcl_CmdInfo info;
  Tcl_HashEntry *entry;
  int new;

  do
    sprintf(interp->result, "hfsfile%d", id++);
  while (Tcl_GetCommandInfo(interp, interp->result, &info));

  fref->file   = file;
  fref->interp = interp;
  fref->cmd    = Tcl_CreateCommand(interp, interp->result,
				   file_cmd, fref, file_del);

  entry = Tcl_CreateHashEntry(&files, (char *) fref, &new);
  Tcl_SetHashValue(entry, vref);
}
