static
int vol_cmd(ClientData clientData, Tcl_Interp *interp,
	    int argc, char *argv[])
{
  volref *vref = clientData;
  hfsvol *vol  = vref->vol;

  switch (argc)
    {
    case 1:
      interp->result = "missing command";
      return TCL_ERROR;

    case 2:
      if (strcmp(argv[1], "vname") == 0)
	{
	  hfsvolent ent;

	  hfs_vstat(vol, &ent);
	  Tcl_SetResult(interp, ent.name, TCL_VOLATILE);
	}
      else if (strcmp(argv[1], "size") == 0)
	{
	  hfsvolent ent;

	  hfs_vstat(vol, &ent);
	  sprintf(interp->result, "%lu %lu", ent.totbytes, ent.freebytes);
	}
      else if (strcmp(argv[1], "crdate") == 0)
	{
	  hfsvolent ent;

	  hfs_vstat(vol, &ent);
	  sprintf(interp->result, "%ld", (long) ent.crdate);
	}
      else if (strcmp(argv[1], "mddate") == 0)
	{
	  hfsvolent ent;

	  hfs_vstat(vol, &ent);
	  sprintf(interp->result, "%ld", (long) ent.mddate);
	}
      else if (strcmp(argv[1], "islocked") == 0)
	{
	  hfsvolent ent;

	  hfs_vstat(vol, &ent);
	  if (ent.flags & HFS_ISLOCKED)
	    interp->result = "1";
	  else
	    interp->result = "0";
	}
      else if (strcmp(argv[1], "umount") == 0)
	{
	  Tcl_DeleteCommand(interp, argv[0]);
	  if (err_umount == -1)
	    return error(interp, 0);
	}
      else if (strcmp(argv[1], "cwd") == 0)
	sprintf(interp->result, "%lu", vref->cwd);
      else if (strcmp(argv[1], "path") == 0)
	{
	  char name[HFS_MAX_FLEN + 1];
	  long id;
	  int listc, i;
	  char **listv;
	  char *result;

	  id = vref->cwd;
	  while (id != HFS_CNID_ROOTPAR)
	    {
	      if (hfs_dirinfo(vol, &id, name) == -1)
		return error(interp, 0);

	      Tcl_AppendElement(interp, name);
	    }

	  /* reverse the resulting list */

	  if (Tcl_SplitList(interp, interp->result, &listc, &listv) != TCL_OK)
	    return TCL_ERROR;

	  for (i = 0; i < listc / 2; ++i)
	    {
	      char *tmp;

	      tmp = listv[i];
	      listv[i] = listv[listc - 1 - i];
	      listv[listc - 1 - i] = tmp;
	    }

	  result = Tcl_Merge(listc, listv);
	  free(listv);

	  Tcl_SetResult(interp, result, TCL_DYNAMIC);
	}
      else if (strcmp(argv[1], "dir") == 0)
	{
	  if (getdir(interp, vref, ":") != TCL_OK)
	    return TCL_ERROR;
	}
      else if (strcmp(argv[1], "flush") == 0)
	{
	  if (hfs_flush(vol) == -1)
	    return error(interp, 0);
	}
      else if (strcmp(argv[1], "sepchar") == 0)
	interp->result = ":";
      else
	{
	  Tcl_AppendResult(interp, "bad command \"", argv[1],
			   "\" or wrong # args", (char *) 0);
	  return TCL_ERROR;
	}
      break;

    case 3:
      if (strcmp(argv[1], "cd") == 0 ||
	  strcmp(argv[1], "chdir") == 0)
	{
	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      hfs_chdir(vol, argv[2]) == -1)
	    return error(interp, 0);

	  vref->cwd = hfs_getcwd(vol);
	}
      else if (strcmp(argv[1], "dirinfo") == 0)
	{
	  long id;
	  char name[HFS_MAX_FLEN + 1], idstr[CHARLEN(unsigned long) + 1];

	  if (Tcl_ExprLong(interp, argv[2], &id) != TCL_OK)
	    return TCL_ERROR;

	  if (hfs_dirinfo(vol, &id, name) == -1)
	    return error(interp, 0);

	  sprintf(idstr, "%lu", id);
	  Tcl_AppendElement(interp, name);
	  Tcl_AppendElement(interp, idstr);
	}
      else if (strcmp(argv[1], "dir") == 0)
	{
	  if (getdir(interp, vref, argv[2]) != TCL_OK)
	    return TCL_ERROR;
	}
      else if (strcmp(argv[1], "open") == 0)
	{
	  fileref *fref;
	  hfsfile *file;

	  fref = ALLOC(fileref, 1);
	  if (fref == 0)
	    {
	      interp->result = "out of memory";
	      return TCL_ERROR;
	    }

	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      (file = hfs_open(vol, argv[2])) == 0)
	    {
	      FREE(fref);
	      return error(interp, 0);
	    }

	  file_ref(interp, vref, fref, file);
	}
      else if (strcmp(argv[1], "stat") == 0)
	{
	  hfsdirent ent;
	  char *str;

	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      hfs_stat(vol, argv[2], &ent) == -1)
	    return error(interp, 0);

	  str = direntstr(&ent);
	  if (str == 0)
	    {
	      interp->result = "out of memory";
	      return TCL_ERROR;
	    }

	  Tcl_SetResult(interp, str, TCL_DYNAMIC);
	}
      else if (strcmp(argv[1], "mkdir") == 0)
	{
	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      hfs_mkdir(vol, argv[2]) == -1)
	    return error(interp, 0);
	}
      else if (strcmp(argv[1], "rmdir") == 0)
	{
	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      hfs_rmdir(vol, argv[2]) == -1)
	    return error(interp, 0);
	}
      else if (strcmp(argv[1], "delete") == 0)
	{
	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      hfs_delete(vol, argv[2]) == -1)
	    return error(interp, 0);
	}
      else if (strcmp(argv[1], "touch") == 0)
	{
	  hfsdirent ent;

	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      hfs_stat(vol, argv[2], &ent) == -1)
	    return error(interp, 0);

	  ent.mddate = time(0);

	  if (hfs_setattr(vol, argv[2], &ent) == -1)
	    return error(interp, 0);
	}
      else if (strcmp(argv[1], "glob") == 0)
	{
	  int listc, fargc;
	  char **listv, **fargv, *result;

	  if (hfs_setcwd(vol, vref->cwd) == -1)
	    return error(interp, 0);

	  if (Tcl_SplitList(interp, argv[2], &listc, &listv) != TCL_OK)
	    return TCL_ERROR;

	  fargv = hfs_glob(vol, listc, listv, &fargc);
	  free(listv);

	  if (fargv == 0)
	    {
	      interp->result = "globbing error";
	      return TCL_ERROR;
	    }

	  result = Tcl_Merge(fargc, fargv);
	  free(fargv);

	  Tcl_SetResult(interp, result, TCL_DYNAMIC);
	}
      else if (strcmp(argv[1], "bless") == 0)
	{
	  hfsvolent volent;
	  hfsdirent dirent;

	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      hfs_vstat(vol, &volent) == -1 ||
	      hfs_stat(vol, argv[2], &dirent) == -1)
	    return error(interp, 0);

	  volent.blessed = dirent.cnid;

	  if (hfs_vsetattr(vol, &volent) == -1)
	    return error(interp, 0);
	}
      else
	{
	  Tcl_AppendResult(interp, "bad command \"", argv[1],
			   "\" or wrong # args", (char *) 0);
	  return TCL_ERROR;
	}
      break;

    case 4:
      if (strcmp(argv[1], "rename") == 0)
	{
	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      hfs_rename(vol, argv[2], argv[3]) == -1)
	    return error(interp, 0);
	}
      else
	{
	  Tcl_AppendResult(interp, "bad command \"", argv[1],
			   "\" or wrong # args", (char *) 0);
	  return TCL_ERROR;
	}
      break;

    case 5:
      if (strcmp(argv[1], "create") == 0)
	{
	  fileref *fref;
	  hfsfile *file;

	  if (strlen(argv[3]) != 4 ||
	      strlen(argv[4]) != 4)
	    {
	      interp->result = "type and creator must be 4 character strings";
	      return TCL_ERROR;
	    }

	  fref = ALLOC(fileref, 1);
	  if (fref == 0)
	    {
	      interp->result = "out of memory";
	      return TCL_ERROR;
	    }

	  if (hfs_setcwd(vol, vref->cwd) == -1 ||
	      (file = hfs_create(vol, argv[2], argv[3], argv[4])) == 0)
	    {
	      FREE(fref);
	      return error(interp, 0);
	    }

	  file_ref(interp, vref, fref, file);
	}
      else if (strcmp(argv[1], "copy") == 0)
	return copynative(interp, vref, argv);
      else if (strcmp(argv[1], "copyin") == 0)
	{
	  if (hfs_setcwd(vol, vref->cwd) == -1)
	    return error(interp, 0);

	  return copyin(interp, vol, argv);
	}
      else if (strcmp(argv[1], "copyout") == 0)
	{
	  if (hfs_setcwd(vol, vref->cwd) == -1)
	    return error(interp, 0);

	  return copyout(interp, vol, argv);
	}
      else
	{
	  Tcl_AppendResult(interp, "bad command \"", argv[1],
			   "\" or wrong # args", (char *) 0);
	  return TCL_ERROR;
	}
      break;

    default:
      Tcl_AppendResult(interp, "bad command \"", argv[1],
		       "\" or wrong # args", (char *) 0);
      return TCL_ERROR;
    }

  return TCL_OK;
}
