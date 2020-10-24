gather_deps (DSO *dso, struct prelink_entry *ent)
{
  int i, j, seen = 0;
  FILE *f = NULL;
  const char *argv[5];
  const char *envp[4];
  char *line = NULL, *p, *q = NULL;
  const char **depends = NULL;
  size_t ndepends = 0, ndepends_alloced = 0;
  size_t len = 0;
  ssize_t n;
  Elf_Scn *scn;
  Elf_Data *data;
  Elf32_Lib *liblist = NULL;
  int nliblist = 0;
  const char *dl;
  const char *ent_filename;

  if (check_dso (dso))
    {
      if (! undo)
	ent->type = ET_UNPRELINKABLE;
      goto error_out;
    }

  ent->pltgot = dso->info[DT_PLTGOT];
  ent->soname = strdup (dso->soname);
  ent->flags = (dso->arch->class == ELFCLASS64 ? PCF_ELF64 : 0)
	       | (dso->arch->machine & PCF_MACHINE);
  if (ent->soname == NULL)
    {
      error (0, ENOMEM, "%s: Could not record SONAME", ent->filename);
      goto error_out;
    }

  dl = dynamic_linker ?: dso->arch->dynamic_linker;
  if (strcmp (dso->filename, dl) == 0
      || is_ldso_soname (dso->soname))
    {
      if (dynamic_info_is_set (dso, DT_GNU_PRELINKED_BIT)
	  && dynamic_info_is_set (dso, DT_CHECKSUM_BIT))
	{
	  if (! undo && dso->arch->read_opd)
	    dso->arch->read_opd (dso, ent);
	  ent->done = 2;
	}
      close_dso (dso);
      return 0;
    }

  for (i = 1; i < dso->ehdr.e_shnum; ++i)
    {
      const char *name;
      if (dso->shdr[i].sh_type == SHT_GNU_LIBLIST
	  && (name = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[i].sh_name))
	  && ! strcmp (name, ".gnu.liblist")
	  && (dso->shdr[i].sh_size % sizeof (Elf32_Lib)) == 0)
	{
	  nliblist = dso->shdr[i].sh_size / sizeof (Elf32_Lib);
	  liblist = (Elf32_Lib *) alloca (dso->shdr[i].sh_size);
	  scn = dso->scn[i];
	  data = elf_getdata (scn, NULL);
	  if (data == NULL || elf_getdata (scn, data)
	      || data->d_buf == NULL || data->d_off
	      || data->d_size != dso->shdr[i].sh_size)
	    liblist = NULL;
	  else
	    memcpy (liblist, data->d_buf, dso->shdr[i].sh_size);
	  if (! undo)
	    break;
	}
      else if (undo
	       && dso->shdr[i].sh_type == SHT_PROGBITS
	       && (name = strptr (dso, dso->ehdr.e_shstrndx,
				  dso->shdr[i].sh_name))
	       && ! strcmp (name, ".gnu.prelink_undo"))
	ent->done = 2;
    }

  if (! undo && dso->arch->read_opd)
    dso->arch->read_opd (dso, ent);
  close_dso (dso);
  dso = NULL;

  i = 0;
  argv[i++] = dl;
  if (ld_library_path)
    {
      argv[i++] = "--library-path";
      argv[i++] = ld_library_path;
    }
  if (strchr (ent->filename, '/') != NULL)
    ent_filename = ent->filename;
  else
    {
      size_t flen = strlen (ent->filename);
      char *tp = alloca (2 + flen + 1);
      memcpy (tp, "./", 2);
      memcpy (tp + 2, ent->filename, flen + 1);
      ent_filename = tp;
    }
  argv[i++] = ent_filename;
  argv[i] = NULL;
  envp[0] = "LD_TRACE_LOADED_OBJECTS=1";
  envp[1] = "LD_TRACE_PRELINKING=1";
  envp[2] = "LD_WARN=";
  envp[3] = NULL;
  f = execve_open (dl, (char * const *)argv, (char * const *)envp);
  if (f == NULL)
    goto error_out;

  do
    {
      n = getline (&line, &len, f);
      if (n < 0)
	break;

      if (line[n - 1] == '\n')
	line[n - 1] = '\0';

      p = strstr (line, " => ");
      if (p)
	{
	  q = strstr (p, " (");
	  if (q == NULL && strcmp (p, " => not found") == 0)
	    {
	      error (0, 0, "%s: Could not find one of the dependencies",
		     ent->filename);
	      goto error_out;
	    }
	}
      if (p == NULL || q == NULL)
	{
	  if (strstr (line, "statically linked") != NULL)
	    error (0, 0, "%s: Library without dependencies", ent->filename);
	  else
	    {
	      p = strstr (line, "error while loading shared libraries: ");
	      if (p != NULL)
		{
		  p += sizeof "error while loading shared libraries: " - 1;
		  q = strstr (line, "cannot open shared object file: "
				    "No such file or directory");
		  if (q != NULL)
		    {
		      error (0, 0,
			     "%s: Could not find one of the dependencies",
			     ent->filename);
		      goto error_out;
		    }
		}
	      error (0, 0, "%s: Could not parse `%s'", ent->filename, line);
	    }
	  goto error_out;
	}

      *p = '\0';
      p += sizeof " => " - 1;
      *q = '\0';
      if (! strcmp (p, ent_filename))
	{
	  ++seen;
	  continue;
	}
      if (ndepends == ndepends_alloced)
	{
	  ndepends_alloced += 10;
	  depends =
	    (const char **) realloc (depends,
				     ndepends_alloced * sizeof (char *));
	  if (depends == NULL)
	    {
	      error (0, ENOMEM, "%s: Could not record dependencies",
		     ent->filename);
	      goto error_out;
	    }
	}

      depends[ndepends] = strdupa (p);
      ++ndepends;
    } while (!feof (f));

  if (execve_close (f))
    {
      f = NULL;
      error (0, 0, "%s: Dependency tracing failed", ent->filename);
      goto error_out;
    }

  f = NULL;
  if (seen != 1)
    {
      error (0, 0, "%s seen %d times in LD_TRACE_PRELINKING output, expected once",
	     ent->filename, seen);
      goto error_out;
    }

  free (line);
  line = NULL;

  if (ndepends == 0)
    ent->depends = NULL;
  else
    {
      ent->depends =
	(struct prelink_entry **)
	malloc (ndepends * sizeof (struct prelink_entry *));
      if (ent->depends == NULL)
	{
	  error (0, ENOMEM, "%s: Could not record dependencies", ent->filename);
	  goto error_out;
	}
    }

  ent->ndepends = ndepends;
  char *cache_dyn_depends = NULL;
  if (ndepends)
    {
      cache_dyn_depends = alloca (ndepends);
      memset (cache_dyn_depends, '\0', ndepends);
    }
  for (i = 0; i < ndepends; ++i)
    {
      ent->depends[i] = prelink_find_entry (depends [i], NULL, 1);
      if (ent->depends[i] == NULL)
	goto error_out_free_depends;

      if (ent->depends[i]->type == ET_CACHE_DYN)
	{
	  ent->depends[i]->type = ET_NONE;
	  free (ent->depends[i]->depends);
	  ent->depends[i]->depends = NULL;
	  ent->depends[i]->ndepends = 0;
	  cache_dyn_depends[i] = 1;
	}

      if (ent->depends[i]->type != ET_NONE
	  && ent->depends[i]->type != ET_BAD
	  && ent->depends[i]->type != ET_DYN
	  && ent->depends[i]->type != ET_UNPRELINKABLE)
	{
	  error (0, 0, "%s is not a shared library", depends [i]);
error_out_regather_libs:
	  for (i = 0; i < ndepends; ++i)
	    {
	      if (cache_dyn_depends[i] && ent->depends[i]->type == ET_NONE)
		gather_lib (ent->depends[i]);
	    }
	  goto error_out_free_depends;
	}
    }

  free (depends);
  depends = NULL;

  for (i = 0; i < ndepends; ++i)
    if (ent->depends[i]->type == ET_NONE
	&& gather_lib (ent->depends[i]))
      {
	cache_dyn_depends[i] = 0;
	goto error_out_regather_libs;
      }

  for (i = 0; i < ndepends; ++i)
    for (j = 0; j < ent->depends[i]->ndepends; ++j)
      if (ent->depends[i]->depends[j] == ent)
	{
	  error (0, 0, "%s has a dependency cycle", ent->canon_filename);
	  goto error_out_free_depends;
	}

  for (i = 0; i < ndepends; ++i)
    if (ent->depends[i]->type == ET_UNPRELINKABLE)
      {
	error (0, 0, "Could not prelink %s because its dependency %s could not be prelinked",
	       ent->filename, ent->depends[i]->filename);
	ent->type = ET_UNPRELINKABLE;
	goto error_out;
      }

  if (! undo && (!nliblist || liblist) && nliblist == ndepends)
    {
      for (i = 0; i < ndepends; ++i)
	if (liblist[i].l_time_stamp != ent->depends[i]->timestamp
	    || liblist[i].l_checksum != ent->depends[i]->checksum
	    || ! ent->depends[i]->done)
	  break;

      if (i == ndepends)
	ent->done = 2;
    }

  return 0;

error_out_free_depends:
  free (ent->depends);
  ent->depends = NULL;
  ent->ndepends = 0;
error_out:
  if (f)
    execve_close (f);
  free (line);
  free (depends);
  if (dso)
    close_dso (dso);
  return 1;
}
