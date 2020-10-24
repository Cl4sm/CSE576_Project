int cmd_hfs(ClientData clientData, Tcl_Interp *interp,
	    int argc, char *argv[])
{
  static int id = 0;

  if (argc < 2)
    {
      interp->result = "wrong # args";
      return TCL_ERROR;
    }

  if (strcmp(argv[1], "mount") == 0)
    {
      int partno;
      hfsvol *vol;
      volref *vref;
      Tcl_CmdInfo info;
      Tcl_HashEntry *entry;
      int new;

      if (argc < 3 || argc > 4)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      if (argc == 4)
	{
	  if (Tcl_GetInt(interp, argv[3], &partno) != TCL_OK)
	    return TCL_ERROR;
	}
      else
	{
	  int nparts;

	  suid_enable();
	  nparts = hfs_nparts(argv[2]);
	  suid_disable();

	  if (nparts > 1)
	    {
	      sprintf(interp->result, "must specify partition number "
		      "(%d available)", nparts);
	      return TCL_ERROR;
	    }
	  else if (nparts == -1)
	    partno = 0;
	  else
	    partno = 1;
	}

      vref = ALLOC(volref, 1);
      if (vref == 0)
	{
	  interp->result = "out of memory";
	  return TCL_ERROR;
	}

      suid_enable();
      vol = hfs_mount(argv[2], partno, HFS_MODE_ANY);
      suid_disable();

      if (vol == 0)
	{
	  error(interp, "can't mount volume");
	  FREE(vref);
	  return TCL_ERROR;
	}

      vref->vol = vol;
      vref->cwd = HFS_CNID_ROOTDIR;

      entry = Tcl_CreateHashEntry(&volumes, (char *) vref, &new);

      do
	sprintf(interp->result, "hfsvol%d", id++);
      while (Tcl_GetCommandInfo(interp, interp->result, &info));

      Tcl_CreateCommand(interp, interp->result,
			vol_cmd, vref, vol_del);
    }
  else if (strcmp(argv[1], "zero") == 0)
    {
      int nparts;
      unsigned long len;

      if (argc != 4)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      if (Tcl_GetInt(interp, argv[3], &nparts) != TCL_OK)
	return TCL_ERROR;

      if (do_zero(argv[2], nparts, &len) == -1)
	return error(interp, 0);

      sprintf(interp->result, "%lu", len);
    }
  else if (strcmp(argv[1], "mkpart") == 0)
    {
      unsigned int len;

      if (argc != 4)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      if (Tcl_GetInt(interp, argv[3], &len) != TCL_OK)
	return TCL_ERROR;

      if (do_mkpart(argv[2], len) == -1)
	return error(interp, 0);
    }
  else if (strcmp(argv[1], "nparts") == 0)
    {
      int nparts;

      if (argc != 3)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      suid_enable();
      nparts = hfs_nparts(argv[2]);
      suid_disable();

      sprintf(interp->result, "%d", nparts);
    }
  else if (strcmp(argv[1], "format") == 0)
    {
      int partno;

      if (argc < 5 || argc > 6)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      if (Tcl_GetInt(interp, argv[3], &partno) != TCL_OK)
	return TCL_ERROR;

      if (argc == 6)
	{
	  int listc, i;
	  char **listv;
	  unsigned long *badblocks;

	  if (Tcl_SplitList(interp, argv[5], &listc, &listv) != TCL_OK)
	    return TCL_ERROR;

	  badblocks = ALLOCX(unsigned long, listc);
	  if (listc && badblocks == 0)
	    {
	      free(listv);

	      interp->result = "out of memory";
	      return TCL_ERROR;
	    }

	  for (i = 0; i < listc; ++i)
	    {
	      if (Tcl_ExprLong(interp, listv[i],
			       (long *) &badblocks[i]) != TCL_OK)
		{
		  free(listv);
		  FREE(badblocks);
		  return TCL_ERROR;
		}
	    }

	  free(listv);

	  if (do_format(argv[2], partno, 0, argv[4], listc, badblocks) == -1)
	    {
	      FREE(badblocks);

	      return error(interp, 0);
	    }

	  FREE(badblocks);
	}
      else
	{
	  if (do_format(argv[2], partno, 0, argv[4], 0, 0) == -1)
	    return error(interp, 0);
	}
    }
  else if (strcmp(argv[1], "flushall") == 0)
    hfs_flushall();
  else if (strcmp(argv[1], "chartrans") == 0)
    {
      char *result;

      if (argc != 5)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      if ((strcmp(argv[2], "latin1") != 0 &&
	   strcmp(argv[2], "macroman") != 0) ||
	  (strcmp(argv[3], "latin1") != 0 &&
	   strcmp(argv[3], "macroman") != 0))
	{
	  interp->result = "bad arg to chartrans: "
	    "charsets must be one of latin1, macroman";
	  return TCL_ERROR;
	}

      if (strcmp(argv[2], "latin1") == 0 &&
	  strcmp(argv[3], "macroman") == 0)
	result = cs_macroman(argv[4], 0);
      else if (strcmp(argv[2], "macroman") == 0 &&
	       strcmp(argv[3], "latin1") == 0)
	result = cs_latin1(argv[4], 0);
      else
	{
	  Tcl_SetResult(interp, argv[4], TCL_VOLATILE);
	  return TCL_OK;
	}

      if (result == 0)
	{
	  interp->result = "out of memory";
	  return TCL_ERROR;
	}

      Tcl_SetResult(interp, result, TCL_DYNAMIC);
    }
  else if (strcmp(argv[1], "version") == 0)
    {
      if (argc != 2)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      interp->result = (char *) hfsutils_version;
    }
  else if (strcmp(argv[1], "copyright") == 0)
    {
      if (argc != 2)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      interp->result = (char *) hfsutils_copyright;
    }
  else if (strcmp(argv[1], "author") == 0)
    {
      if (argc != 2)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      interp->result = (char *) hfsutils_author;
    }
  else if (strcmp(argv[1], "license") == 0)
    {
      if (argc != 2)
	{
	  interp->result = "wrong # args";
	  return TCL_ERROR;
	}

      interp->result = (char *) hfsutils_license;
    }
  else
    {
      Tcl_AppendResult(interp, "bad hfs command \"", argv[1],
		       "\": should be one of "
		       "mount, zero, mkpart, format, flushall, "
		       "version, copyright, author, license",
		       (char *) 0);
      return TCL_ERROR;
    }

  return TCL_OK;
}
