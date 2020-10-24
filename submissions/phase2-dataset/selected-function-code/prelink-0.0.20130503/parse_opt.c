static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  char *endarg;

  switch (key)
    {
    case 'a':
      all = 1;
      break;
    case 'b':
      if (add_to_blacklist (arg, dereference, one_file_system))
	exit (EXIT_FAILURE);
      break;
    case 'f':
      force = 1;
      break;
    case 'p':
      print_cache = 1;
      break;
    case 'q':
      quick = 1;
      break;
    case 'v':
      ++verbose;
      break;
    case 'T':
      timestamp_output = 1;
      break;
    case 'R':
      random_base |= 1;
      break;
    case OPT_SEED:
      random_base |= 2;
      seed = strtoull (arg, &endarg, 0);
      if (endarg != strchr (arg, '\0'))
	error (EXIT_FAILURE, 0, "--seed option requires numberic argument");
      break;
    case 'r':
      reloc_only = 1;
      reloc_base = strtoull (arg, &endarg, 0);
      if (endarg != strchr (arg, '\0'))
	error (EXIT_FAILURE, 0, "-r option requires numberic argument");
      break;
    case 'h':
      dereference = 1;
      break;
    case 'l':
      one_file_system = 1;
      break;
    case 'm':
      conserve_memory = 1;
      break;
    case 'N':
      no_update = 1;
      break;
    case 'n':
      dry_run = 1;
      break;
    case 'C':
      prelink_cache = arg;
      break;
    case 'c':
      prelink_conf = arg;
      break;
    case 'u':
      undo = 1;
      break;
    case 'y':
      verify = 1;
      break;
    case 'o':
      undo_output = arg;
      break;
    case OPT_DYNAMIC_LINKER:
      dynamic_linker = arg;
      break;
    case OPT_LD_LIBRARY_PATH:
      ld_library_path = arg;
      break;
    case OPT_LIBS_ONLY:
      libs_only = 1;
      break;
    case OPT_MD5:
      verify_method = VERIFY_MD5;
      break;
    case OPT_SHA:
      verify_method = VERIFY_SHA;
      break;
    case OPT_CXX_DISABLE:
      enable_cxx_optimizations = 0;
      break;
    case OPT_MMAP_REG_START:
      mmap_reg_start = strtoull (arg, &endarg, 0);
      if (endarg != strchr (arg, '\0'))
	error (EXIT_FAILURE, 0, "--mmap-region-start option requires numberic argument");
      break;
    case OPT_MMAP_REG_END:
      mmap_reg_end = strtoull (arg, &endarg, 0);
      if (endarg != strchr (arg, '\0'))
	error (EXIT_FAILURE, 0, "--mmap-region-end option requires numberic argument");
      break;
    case OPT_EXEC_SHIELD:
      exec_shield = 1;
      break;
    case OPT_NO_EXEC_SHIELD:
      exec_shield = 0;
      break;
    case OPT_COMPUTE_CHECKSUM:
      compute_checksum = 1;
      break;
    case OPT_LAYOUT_PAGE_SIZE:
      layout_page_size = strtoull (arg, &endarg, 0);
      if (endarg != strchr (arg, '\0') || (layout_page_size & (layout_page_size - 1)))
	error (EXIT_FAILURE, 0, "--layout-page-size option requires numberic power-of-two argument");
      break;
    case 'i':
      noreexecinit=1;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}
