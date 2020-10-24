static
void vol_del(ClientData clientData)
{
  volref *vref = clientData;
  Tcl_HashEntry *entry;

  entry = Tcl_FindHashEntry(&volumes, (char *) vref);
  if (entry)
    Tcl_DeleteHashEntry(entry);

  do
    {
      Tcl_HashSearch search;

      for (entry = Tcl_FirstHashEntry(&files, &search); entry;
	   entry = Tcl_NextHashEntry(&search))
	{
	  if (Tcl_GetHashValue(entry) == vref)
	    {
	      fileref *fref = (fileref *) Tcl_GetHashKey(&files, entry);

	      Tcl_DeleteCommand(fref->interp,
				Tcl_GetCommandName(fref->interp, fref->cmd));
	      break;
	    }
	}
    }
  while (entry);

  err_umount = hfs_umount(vref->vol);

  FREE(vref);
}
