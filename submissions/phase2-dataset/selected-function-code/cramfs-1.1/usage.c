{
	FILE *stream = status ? stderr : stdout;

	fprintf(stream, "usage: %s [-h] [-b blksize] [-e edition] [-i file] [-n name] dirname outfile\n"
		" -h         print this help\n"
		" -E         make all warnings errors (non-zero exit status)\n"
		" -b blksize blocksize to use\n"
		" -e edition set edition number (part of fsid)\n"
		" -i file    insert a file image into the filesystem (requires >= 2.4.0)\n"
		" -n name    set name of cramfs filesystem\n"
		" -p         pad by %d bytes for boot code\n"
		" -s         sort directory entries (old option, ignored)\n"
		" -v         be more verbose\n"
		" -z         make explicit holes (requires >= 2.3.39)\n"
		" dirname    root of the directory tree to be compressed\n"
		" outfile    output file\n", progname, PAD_SIZE);

	exit(status);
}
