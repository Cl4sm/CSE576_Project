int copynative(Tcl_Interp *interp, volref *srcvref, char *argv[])
{
  volref *dstvref;
  Tcl_CmdInfo info;
  Tcl_HashEntry *entry = 0;
  hfsdirent ent;
  const char *srcname, *dstname;
  hfsfile *ifile, *ofile;
  int result;
  unsigned long cnid;

  if (Tcl_GetCommandInfo(interp, argv[3], &info))
    entry = Tcl_FindHashEntry(&volumes, (char *) info.clientData);

  if (entry == 0)
    {
      Tcl_AppendResult(interp, "unknown volume \"", argv[3], "\"", (char *) 0);
      return TCL_ERROR;
    }

  dstvref = info.clientData;

  srcname = argv[2];
  dstname = argv[4];

  if (hfs_setcwd(srcvref->vol, srcvref->cwd) == -1)
    return error(interp, 0);

  ifile = hfs_open(srcvref->vol, srcname);
  if (ifile == 0)
    return error(interp, "can't open source file");

  if (hfs_setcwd(dstvref->vol, dstvref->cwd) == -1)
    {
      error(interp, 0);
      hfs_close(ifile);
      return TCL_ERROR;
    }

  cnid = 0;

  if (hfs_stat(dstvref->vol, dstname, &ent) != -1)
    {
      if (ent.flags & HFS_ISDIR)
	{
	  if (hfs_setcwd(dstvref->vol, ent.cnid) == -1)
	    {
	      error(interp, 0);
	      hfs_close(ifile);
	      return TCL_ERROR;
	    }

	  dstname = srcname;

	  if (hfs_stat(dstvref->vol, dstname, &ent) != -1)
	    cnid = ent.cnid;
	}
      else
	cnid = ent.cnid;
    }

  if (hfs_fstat(ifile, &ent) == -1)
    {
      error(interp, "can't stat source file");
      hfs_close(ifile);
      return TCL_ERROR;
    }

  if (srcvref->vol == dstvref->vol &&
      ent.cnid == cnid)
    {
      interp->result = "source and destination files are the same";
      hfs_close(ifile);
      return TCL_ERROR;
    }

  hfs_delete(dstvref->vol, dstname);

  ofile = hfs_create(dstvref->vol, dstname,
		     ent.u.file.type, ent.u.file.creator);
  if (ofile == 0)
    {
      error(interp, "can't create destination file");
      hfs_close(ifile);
      return TCL_ERROR;
    }

  result = do_copynative(interp, ifile, ofile);

  ent.fdflags &= ~(HFS_FNDR_ISONDESK | HFS_FNDR_HASBEENINITED);

  if (result == TCL_OK && hfs_fsetattr(ofile, &ent) == -1)
    result = error(interp, "can't set destination file attributes");

  if (hfs_close(ofile) == -1 && result == TCL_OK)
    result = error(interp, "error closing destination file");

  if (hfs_close(ifile) == -1 && result == TCL_OK)
    result = error(interp, "error closing source file");

  return result;
}
