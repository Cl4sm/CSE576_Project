int file_cmd(ClientData clientData, Tcl_Interp *interp,
	     int argc, char *argv[])
{
  fileref *fref = clientData;
  hfsfile *file = fref->file;

  switch (argc)
    {
    case 1:
      interp->result = "missing command";
      return TCL_ERROR;

    case 2:
      if (strcmp(argv[1], "close") == 0)
	{
	  Tcl_DeleteCommand(interp, argv[0]);
	  if (err_close == -1)
	    return error(interp, 0);
	}
      else if (strcmp(argv[1], "tell") == 0)
	{
	  long offset;

	  offset = hfs_seek(file, 0, HFS_SEEK_CUR);
	  if (offset == -1)
	    return error(interp, 0);

	  sprintf(interp->result, "%lu", offset);
	}
      else if (strcmp(argv[1], "stat") == 0)
	{
	  hfsdirent ent;
	  char *str;

	  if (hfs_fstat(file, &ent) == -1)
	    return error(interp, 0);

	  str = direntstr(&ent);
	  if (str == 0)
	    {
	      interp->result = "out of memory";
	      return TCL_ERROR;
	    }

	  Tcl_SetResult(interp, str, TCL_DYNAMIC);
	}
      else if (strcmp(argv[1], "getfork") == 0)
	{
	  interp->result = (hfs_getfork(file) == 0) ? "data" : "rsrc";
	}
      else
	{
	  Tcl_AppendResult(interp, "bad command \"", argv[1],
			   "\" or wrong # args", (char *) 0);
	  return TCL_ERROR;
	}
      break;

    case 3:
      if (strcmp(argv[1], "setfork") == 0 ||
	  strcmp(argv[1], "fork") == 0)
	{
	  int fork;

	  if (strcmp(argv[2], "data") == 0)
	    fork = 0;
	  else if (strcmp(argv[2], "rsrc") == 0 ||
		   strcmp(argv[2], "resource") == 0)
	    fork = 1;
	  else
	    {
	      interp->result = "bad arg to setfork: must be data or rsrc";
	      return TCL_ERROR;
	    }

	  hfs_setfork(file, fork);
	}
      else if (strcmp(argv[1], "seek") == 0)
	{
	  long offset;

	  if (Tcl_ExprLong(interp, argv[2], &offset) != TCL_OK)
	    return TCL_ERROR;

	  offset = hfs_seek(file, offset, HFS_SEEK_SET);
	  if (offset == -1)
	    return error(interp, 0);

	  sprintf(interp->result, "%lu", offset);
	}
      else if (strcmp(argv[1], "read") == 0)
	{
	  long bytes;
	  char *mem;

	  if (Tcl_ExprLong(interp, argv[2], &bytes) != TCL_OK)
	    return TCL_ERROR;

	  if (bytes < 0)
	    {
	      interp->result = "size must be >= 0";
	      return TCL_ERROR;
	    }

	  mem = ALLOC(char, bytes + 1);
	  if (mem == 0)
	    {
	      interp->result = "out of memory";
	      return TCL_ERROR;
	    }

	  bytes = hfs_read(file, mem, bytes);
	  if (bytes == -1)
	    {
	      free(mem);
	      return error(interp, 0);
	    }

	  mem[bytes] = 0;

	  Tcl_SetResult(interp, mem, TCL_DYNAMIC);
	}
      else if (strcmp(argv[1], "write") == 0)
	{
	  long bytes;

	  bytes = hfs_write(file, argv[2], strlen(argv[2]));
	  if (bytes == -1)
	    return error(interp, 0);

	  sprintf(interp->result, "%lu", bytes);
	}
      else
	{
	  Tcl_AppendResult(interp, "bad command \"", argv[1],
			   "\" or wrong # args", (char *) 0);
	  return TCL_ERROR;
	}
      break;

    case 4:
      if (strcmp(argv[1], "seek") == 0)
	{
	  long offset;
	  int whence;

	  if (Tcl_ExprLong(interp, argv[2], &offset) != TCL_OK)
	    return TCL_ERROR;

	  if (strcmp(argv[3], "start") == 0 ||
	      strcmp(argv[3], "set") == 0)
	    whence = HFS_SEEK_SET;
	  else if (strcmp(argv[3], "current") == 0 ||
		   strcmp(argv[3], "cur") == 0)
	    whence = HFS_SEEK_CUR;
	  else if (strcmp(argv[3], "end") == 0)
	    whence = HFS_SEEK_END;
	  else
	    {
	      interp->result = "bad arg 3: must be start, current, or end";
	      return TCL_ERROR;
	    }

	  offset = hfs_seek(file, offset, whence);
	  if (offset == -1)
	    return error(interp, 0);

	  sprintf(interp->result, "%lu", offset);
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
