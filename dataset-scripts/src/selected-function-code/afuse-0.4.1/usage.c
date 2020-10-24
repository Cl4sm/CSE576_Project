static void usage(const char *progname)
{
	fprintf(stderr,
		"Usage: %s mountpoint [options]\n"
		"\n"
		"    -o opt,[opt...]        mount options\n"
		"    -h   --help            print help\n"
		"    -V   --version         print FUSE version information\n"
		"\n"
		"afuse options:\n"
		"    -o mount_template=CMD         template for CMD to execute to mount (1)\n"
		"    -o unmount_template=CMD       template for CMD to execute to unmount (1) (2)\n"
		"    -o populate_root_command=CMD  CMD to execute providing root directory list (3)\n"
		"    -o filter_file=FILE           FILE listing ignore filters for mount points (4)\n"
		"    -o timeout=TIMEOUT            automatically unmount after TIMEOUT seconds\n"
		"    -o flushwrites                flushes data to disk for all file writes\n"
		"    -o exact_getattr              allows getattr calls to cause a mount\n" 
		"\n\n"
		" (1) - When executed, %%r and %%m are expanded in templates to the root\n"
		"       directory name for the new mount point, and the actual directory to\n"
		"       mount onto respectively to mount onto. Both templates are REQUIRED.\n"
		"\n"
		" (2) - The unmount command must perform a lazy unmount operation. E.g. the\n"
		"       -u -z options to fusermount, or -l for regular mount.\n"
		"\n"
		" (3) - The populate_root command should output one dir entry per line,\n"
		"       and return immediately. It is run for each directory listing request.\n"
		"\n"
		" (4) - Each line of the filter file is a shell wildcard filter (glob). A '#'\n"
		"       as the first character on a line ignores a filter.\n"
		"\n"
		" The following filter patterns are hard-coded:"
		"\n", progname);

	mount_filter_list_t *cur = mount_filter_list;
	while (cur) {
		fprintf(stderr, "    %s\n", cur->pattern);
		cur = cur->next;
	}

	fprintf(stderr, "\n");
}