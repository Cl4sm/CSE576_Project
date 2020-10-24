int getdir(Tcl_Interp *interp, volref *vref, const char *path)
{
  hfsvol *vol = vref->vol;
  hfsdir *dir;
  hfsdirent ent;
  char *str;

  if (hfs_setcwd(vol, vref->cwd) == -1 ||
      hfs_stat(vol, path, &ent) == -1)
    return error(interp, 0);

  if (ent.flags & HFS_ISDIR)
    {
      dir = hfs_opendir(vol, path);
      if (dir == 0)
	return error(interp, 0);

      while (hfs_readdir(dir, &ent) != -1)
	{
	  str = direntstr(&ent);
	  if (str == 0)
	    {
	      hfs_closedir(dir);
	      Tcl_SetResult(interp, "out of memory", TCL_STATIC);
	      return TCL_ERROR;
	    }

	  Tcl_AppendElement(interp, str);

	  free(str);
	}

      if (hfs_closedir(dir) == -1)
	return error(interp, 0);
    }
  else  /* ! HFS_ISDIR */
    {
      str = direntstr(&ent);
      if (str == 0)
	{
	  interp->result = "out of memory";
	  return TCL_ERROR;
	}

      Tcl_AppendElement(interp, str);

      free(str);
    }

  return TCL_OK;
}
