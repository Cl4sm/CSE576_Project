int main(int argc, char **argv)
{
	struct stat st;		/* used twice... */
	struct entry *root_entry;
	char *rom_image;
	ssize_t offset, written;
	int fd;
	/* initial guess (upper-bound) of required filesystem size */
	loff_t fslen_ub = sizeof(struct cramfs_super);
	char const *dirname, *outfile;
	u32 crc;
	int c;			/* for getopt */
	char *ep;		/* for strtoul */

	blksize = sysconf(_SC_PAGESIZE);
	total_blocks = 0;

	if (argc)
		progname = argv[0];

	/* command line options */
	while ((c = getopt(argc, argv, "hEb:e:i:n:psvz")) != EOF) {
		switch (c) {
		case 'h':
			usage(MKFS_OK);
		case 'E':
			opt_errors = 1;
			break;
		case 'b':
			errno = 0;
			blksize = strtoul(optarg, &ep, 10);
			if (errno || optarg[0] == '\0' || *ep != '\0')
				usage(MKFS_USAGE);
			if (blksize < 512 || (blksize & (blksize - 1)))
				die(MKFS_ERROR, 0, "invalid blocksize: %u", blksize);
			break;
		case 'e':
			errno = 0;
			opt_edition = strtoul(optarg, &ep, 10);
			if (errno || optarg[0] == '\0' || *ep != '\0')
				usage(MKFS_USAGE);
			break;
		case 'i':
			opt_image = optarg;
			if (lstat(opt_image, &st) < 0) {
				die(MKFS_ERROR, 1, "lstat failed: %s", opt_image);
			}
			image_length = st.st_size; /* may be padded later */
			fslen_ub += (image_length + 3); /* 3 is for padding */
			break;
		case 'n':
			opt_name = optarg;
			break;
		case 'p':
			opt_pad = PAD_SIZE;
			fslen_ub += PAD_SIZE;
			break;
		case 's':
			/* old option, ignored */
			break;
		case 'v':
			opt_verbose++;
			break;
		case 'z':
			opt_holes = 1;
			break;
		}
	}

	if ((argc - optind) != 2)
		usage(MKFS_USAGE);
	dirname = argv[optind];
	outfile = argv[optind + 1];

	if (stat(dirname, &st) < 0) {
		die(MKFS_USAGE, 1, "stat failed: %s", dirname);
	}
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd < 0) {
		die(MKFS_USAGE, 1, "open failed: %s", outfile);
	}

	root_entry = calloc(1, sizeof(struct entry));
	if (!root_entry) {
		die(MKFS_ERROR, 1, "calloc failed");
	}
	root_entry->mode = st.st_mode;
	root_entry->uid = st.st_uid;
	root_entry->gid = st.st_gid;

	root_entry->size = parse_directory(root_entry, dirname, &root_entry->child, &fslen_ub);

	/* always allocate a multiple of blksize bytes because that's
	   what we're going to write later on */
	fslen_ub = ((fslen_ub - 1) | (blksize - 1)) + 1;

	if (fslen_ub > MAXFSLEN) {
		fprintf(stderr,
			"warning: estimate of required size (upper bound) is %jdMB, but maximum image size is %uMB, we might die prematurely\n",
			(intmax_t) (fslen_ub >> 20),
			MAXFSLEN >> 20);
		fslen_ub = MAXFSLEN;
	}

	/* find duplicate files. TODO: uses the most inefficient algorithm
	   possible. */
	eliminate_doubles(root_entry, root_entry);

	/* TODO: Why do we use a private/anonymous mapping here
	   followed by a write below, instead of just a shared mapping
	   and a couple of ftruncate calls?  Is it just to save us
	   having to deal with removing the file afterwards?  If we
	   really need this huge anonymous mapping, we ought to mmap
	   in smaller chunks, so that the user doesn't need nn MB of
	   RAM free.  If the reason is to be able to write to
	   un-mmappable block devices, then we could try shared mmap
	   and revert to anonymous mmap if the shared mmap fails. */
	rom_image = mmap(NULL, fslen_ub?fslen_ub:1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (rom_image == MAP_FAILED) {
		die(MKFS_ERROR, 1, "mmap failed");
	}

	/* Skip the first opt_pad bytes for boot loader code */
	offset = opt_pad;
	memset(rom_image, 0x00, opt_pad);

	/* Skip the superblock and come back to write it later. */
	offset += sizeof(struct cramfs_super);

	/* Insert a file image. */
	if (opt_image) {
		printf("Including: %s\n", opt_image);
		offset = write_file(opt_image, rom_image, offset);
	}

	offset = write_directory_structure(root_entry->child, rom_image, offset);
	printf("Directory data: %zd bytes\n", offset);

	offset = write_data(root_entry, rom_image, offset);

	/* We always write a multiple of blksize bytes, so that
	   losetup works. */
	offset = ((offset - 1) | (blksize - 1)) + 1;
	printf("Everything: %zd kilobytes\n", offset >> 10);

	/* Write the superblock now that we can fill in all of the fields. */
	write_superblock(root_entry, rom_image+opt_pad, offset);
	printf("Super block: %zd bytes\n", sizeof(struct cramfs_super));

	/* Put the checksum in. */
	crc = crc32(0L, Z_NULL, 0);
	crc = crc32(crc, (rom_image+opt_pad), (offset-opt_pad));
	((struct cramfs_super *) (rom_image+opt_pad))->fsid.crc = crc;
	printf("CRC: %x\n", crc);

	/* Check to make sure we allocated enough space. */
	if (fslen_ub < offset) {
		die(MKFS_ERROR, 0, "not enough space allocated for ROM image (%Ld allocated, %d used)", fslen_ub, offset);
	}

	written = write(fd, rom_image, offset);
	if (written < 0) {
		die(MKFS_ERROR, 1, "write failed");
	}
	if (offset != written) {
		die(MKFS_ERROR, 0, "ROM image write failed (wrote %d of %d bytes): No space left on device?", written, offset);
	}

	/* (These warnings used to come at the start, but they scroll off the
	   screen too quickly.) */
	if (warn_namelen)
		fprintf(stderr, /* bytes, not chars: think UTF-8. */
			"warning: filenames truncated to %d bytes (possibly less if multi-byte UTF-8)\n",
			CRAMFS_MAXPATHLEN);
	if (warn_skip)
		fprintf(stderr, "warning: files were skipped due to errors\n");
	if (warn_size)
		fprintf(stderr,
			"warning: file sizes truncated to %luMB (minus 1 byte)\n",
			1L << (CRAMFS_SIZE_WIDTH - 20));
	if (warn_uid) /* (not possible with current Linux versions) */
		fprintf(stderr,
			"warning: uids truncated to %u bits (this may be a security concern)\n",
			CRAMFS_UID_WIDTH);
	if (warn_gid)
		fprintf(stderr,
			"warning: gids truncated to %u bits (this may be a security concern)\n",
			CRAMFS_GID_WIDTH);
	if (warn_dev)
		fprintf(stderr,
			"WARNING: device numbers truncated to %u bits (this almost certainly means\n"
			"that some device files will be wrong)\n",
			CRAMFS_OFFSET_WIDTH);
	if (opt_errors &&
	    (warn_namelen||warn_skip||warn_size||warn_uid||warn_gid||warn_dev))
		exit(MKFS_ERROR);

	exit(MKFS_OK);
}
