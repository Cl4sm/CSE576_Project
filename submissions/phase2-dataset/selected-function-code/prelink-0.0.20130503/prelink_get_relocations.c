prelink_get_relocations (struct prelink_info *info)
{
  FILE *f;
  DSO *dso = info->dso;
  const char *argv[5];
  const char *envp[4];
  int i, ret, status;
  char *p;
  const char *dl = dynamic_linker ?: dso->arch->dynamic_linker;
  const char *ent_filename;

  if (info->ent->type == ET_DYN)
    {
      assert (info->ent->base == dso->base);
      if (info->ent->end < dso->end)
	{
	  error (0, 0, "%s: grew since it has been recorded", info->ent->filename);
	  return 0;
	}
    }
  else
    {
      info->ent->base = dso->base;
      info->ent->end = dso->end;
    }

  if (is_ldso_soname (info->dso->soname))
    return 1;

  info->symbol_count = (info->symtab_end - info->symtab_start)
		       / info->symtab_entsize;
  info->symbols = calloc (sizeof (struct prelink_symbol), info->symbol_count);

  i = 0;
  argv[i++] = dl;
  if (ld_library_path)
    {
      argv[i++] = "--library-path";
      argv[i++] = ld_library_path;
    }
  if (strchr (info->ent->filename, '/') != NULL)
    ent_filename = info->ent->filename;
  else
    {
      size_t flen = strlen (info->ent->filename);
      char *p = alloca (2 + flen + 1);
      memcpy (p, "./", 2);
      memcpy (p + 2, info->ent->filename, flen + 1);
      ent_filename = p;
    }
  argv[i++] = ent_filename;
  argv[i] = NULL;
  envp[0] = "LD_TRACE_LOADED_OBJECTS=1";
  envp[1] = "LD_BIND_NOW=1";
  p = alloca (sizeof "LD_TRACE_PRELINKING=" + strlen (info->ent->filename));
  strcpy (stpcpy (p, "LD_TRACE_PRELINKING="), info->ent->filename);
  envp[2] = p;
  envp[3] = NULL;

  ret = 2;
  f = execve_open (dl, (char * const *)argv, (char * const *)envp);
  if (f == NULL)
    {
      error (0, errno, "%s: Could not trace symbol resolving",
	     info->ent->filename);
      return 0;
    }

  if (prelink_record_relocations (info, f, ent_filename))
    ret = 0;

  if ((status = execve_close (f)))
    {
      if (ret)
	error (0, status == -1 ? errno : 0,
	       "%s Could not trace symbol resolving", info->ent->filename);
      return 0;
    }

  return ret;
}
