static
int process(hfsvol *vol, darray *dirs, darray *files,
	    int flags, int options, int width)
{
  int i, dsz, fsz;
  queueent *ents;
  int result = 0;

  dsz = darr_size(dirs);
  fsz = darr_size(files);

  if (fsz)
    {
      sortfiles(files, flags, options);
      if (showfiles(files, flags, options, width) == -1)
	result = -1;

      flags |= HLS_NAME | HLS_SPACE;
    }
  else if (dsz > 1)
    flags |= HLS_NAME;

  ents = darr_array(dirs);

  for (i = 0; i < dsz; ++i)
    {
      const char *path;
      hfsdir *dir;
      queueent ent;

      darr_shrink(files, 0);

      path = PATH(ents[i]);
      dir  = hfs_opendir(vol, path);
      if (dir == 0)
	{
	  hfsutil_perrorp(path);
	  result = -1;
	  continue;
	}

      while (hfs_readdir(dir, &ent.dirent) != -1)
	{
	  if ((ent.dirent.fdflags & HFS_FNDR_ISINVISIBLE) &&
	      ! (flags & HLS_ALL_FILES))
	    continue;

	  ent.path = 0;
	  ent.free = 0;

	  if (darr_append(files, &ent) == 0)
	    {
	      fprintf(stderr, "%s: not enough memory\n", argv0);
	      result = -1;
	      break;
	    }

	  if ((ent.dirent.flags & HFS_ISDIR) && (flags & HLS_RECURSIVE))
	    {
	      dstring str;

	      dstr_init(&str);

	      if (strchr(path, ':') == 0 &&
		  dstr_append(&str, ":", 1) == -1)
		result = -1;

	      if (dstr_append(&str, path, -1) == -1)
		result = -1;

	      if (path[strlen(path) - 1] != ':' &&
		  dstr_append(&str, ":", 1) == -1)
		result = -1;

	      if (dstr_append(&str, ent.dirent.name, -1) == -1)
		result = -1;

	      ent.path = strdup(dstr_string(&str));
	      if (ent.path)
		ent.free = dpfree;
	      else
		result = -1;

	      dstr_free(&str);

	      if (darr_append(dirs, &ent) == 0)
		{
		  result = -1;
		  if (ent.path)
		    free(ent.path);
		}

	      if (result)
		{
		  fprintf(stderr, "%s: not enough memory\n", argv0);
		  break;
		}

	      dsz  = darr_size(dirs);
	      ents = darr_array(dirs);
	    }
	}

      hfs_closedir(dir);

      if (result)
	break;

      if (flags & HLS_SPACE)
	printf("\n");
      if (flags & HLS_NAME)
	printf("%s%s", path,
	       path[strlen(path) - 1] == ':' ? "\n" : ":\n");

      sortfiles(files, flags, options);
      if (showfiles(files, flags, options, width) == -1)
	result = -1;

      flags |= HLS_NAME | HLS_SPACE;
    }

  return result;
}
