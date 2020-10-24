int
main (int argc, char *argv[])
{
  int remaining, failures = 0;

  setlocale (LC_ALL, "");

  error_print_progname = print_error_prefix;

  /* Set the default for exec_shield.  */
  if (! access ("/proc/sys/kernel/exec-shield", F_OK))
    exec_shield = 1;

  set_default_layout_page_size ();

  prelink_init_cache ();

  elf_version (EV_CURRENT);

  argp_parse (&argp, argc, argv, 0, &remaining, 0);

  if(!noreexecinit) {
    initctime = get_ctime("/sbin/init");
       atexit(checkinit);
  }

  if (ld_library_path == NULL)
    ld_library_path = getenv ("LD_LIBRARY_PATH");

  if (all && reloc_only)
    error (EXIT_FAILURE, 0, "--all and --reloc-only options are incompatible");
  if ((undo || verify) && reloc_only)
    error (EXIT_FAILURE, 0, "--undo and --reloc-only options are incompatible");
  if (verify && (undo || all))
    error (EXIT_FAILURE, 0, "--verify and either --undo or --all options are incompatible");
  if (dry_run && verify)
    error (EXIT_FAILURE, 0, "--dry-run and --verify options are incompatible");
  if ((undo || verify) && quick)
    error (EXIT_FAILURE, 0, "--undo and --quick options are incompatible");

  if (print_cache)
    {
      prelink_load_cache ();
      prelink_print_cache ();
      return 0;
    }

  if (remaining == argc && ! all)
    error (EXIT_FAILURE, 0, "no files given and --all not used");

  if (undo_output && (!undo || all))
    error (EXIT_FAILURE, 0, "-o can be only specified together with -u and without -a");

  if (undo_output && remaining + 1 != argc)
    error (EXIT_FAILURE, 0, "-o can only be used when undoing a single object");

  if (compute_checksum)
    {
      while (remaining < argc)
	{
	  DSO *dso = open_dso (argv[remaining++]);

	  if (dso == NULL || reopen_dso (dso, NULL, NULL)
	      || prelink_set_checksum (dso))
	    error (0, 0, "could not recompute checksum of %s", dso->filename);
	  close_dso (dso);
	  error (0, 0, "%08x %s\n", (unsigned int) dso->info_DT_CHECKSUM, dso->filename);
	}
      exit (0);
    }

  if (verify)
    {
      if (remaining + 1 != argc)
	error (EXIT_FAILURE, 0, "only one library or binary can be verified in a single command");
      return prelink_verify (argv[remaining]);
    }

  if (reloc_only || (undo && ! all))
    {
      while (remaining < argc)
	{
	  DSO *dso = open_dso (argv[remaining++]);
	  int ret;

	  if (dso == NULL)
	    {
	      ++failures;
	      continue;
	    }

	  if (dso->ehdr.e_type != ET_DYN
	      && (reloc_only || dso->ehdr.e_type != ET_EXEC))
	    {
	      ++failures;
	      error (0, 0, "%s is not a shared library", dso->filename);
	      continue;
	    }

	  if (undo)
	    ret = prelink_undo (dso);
	  else
	    ret = relocate_dso (dso, reloc_base);

	  if (ret)
	    {
	      ++failures;
	      close_dso (dso);
	      continue;
	    }

	  if (dynamic_info_is_set (dso, DT_CHECKSUM_BIT)
	      && dso_is_rdwr (dso)
	      && prelink_set_checksum (dso))
	    {
	      ++failures;
	      close_dso (dso);
	      continue;
	    }

	  if (dry_run)
	    {
	      close_dso (dso);
	      continue;
	    }

	  if (reloc_only)
	    dso->permissive = 1;
	  else if (undo_output)
	    {
	      const char *output, *orig_filename;

	      if (!dso_is_rdwr (dso))
		{
		  struct stat64 st;
		  int err;

		  if (fstat64 (dso->fd, &st) < 0)
		    {
		      error (0, errno, "Could not stat %s", dso->filename);
		      ++failures;
		      close_dso (dso);
		      continue;
		    }
		  err = copy_fd_to_file (dso->fd, undo_output, &st);
		  if (err)
		    {
		      error (0, err, "Could not undo %s to %s", dso->filename,
			     undo_output);
		      ++failures;
		    }
		  close_dso (dso);
		  continue;
		}

	      output = strdup (undo_output);
	      if (!output)
		{
		  ++failures;
		  close_dso (dso);
		  continue;
		}
	      if (dso->filename != dso->soname)
		orig_filename = dso->filename;
	      else
		orig_filename = strdup (dso->filename);
	      if (!orig_filename)
		{
		  ++failures;
		  close_dso (dso);
		  continue;
		}
	      dso->filename = output;
	      if (update_dso (dso, orig_filename))
		++failures;
	      free ((char *) orig_filename);
	      continue;
	    }

	  if (update_dso (dso, NULL))
	    ++failures;
	}

      return failures;
    }

  if (read_config (prelink_conf))
    return EXIT_FAILURE;

  if (blacklist_from_config ())
    return EXIT_FAILURE;

  if (quick)
    prelink_load_cache ();

  if (gather_config ())
    return EXIT_FAILURE;

  while (remaining < argc)
    if (gather_object (argv[remaining++], dereference, one_file_system))
      return EXIT_FAILURE;

  if (gather_check_libs ())
    return EXIT_FAILURE;

  if (undo)
    return undo_all ();

  if (! all && ! quick)
    prelink_load_cache ();

  layout_libs ();
  prelink_all ();

  if (! no_update && ! dry_run)
    prelink_save_cache (all);
  return 0;
}
