iow_t *lzo_wopen(iow_t *child, int compress_level)
{
	const int opt_filter = 0;
	int flags;
	iow_t *iow;
	struct buffer_t buffer;
	buffer.offset=0;
	int i;

	if (!child)
		return NULL;

	if (lzo_init() != LZO_E_OK) {
		/* Fail */
		return NULL;
	}

        /* Compress level is useless for LZO, but getting UNUSED into here
         * is more trouble than it is worth so this check will at least
         * stop us from getting warnings about it.
         */
        if (compress_level < 0)
                return NULL;

	iow = malloc(sizeof(iow_t));
	iow->source = &lzo_wsource;
	iow->data = malloc(sizeof(struct lzow_t));

	DATA(iow)->child = child;
	DATA(iow)->err = ERR_OK;

	flags = 0;
	flags |= F_OS_UNIX & F_OS_MASK;	/* Operating System */
	flags |= F_CS_NATIVE & F_CS_MASK;	/* Character Set */
	flags |= F_ADLER32_D; /* We adler32 the uncompressed data */
	/* flags |= F_STDIN; */
	/* flags |= F_STDOUT */
	/* flags |= F_MULTIPART; */
	/* flags |= F_H_CRC32; */

	write_buf(&buffer, lzop_magic, sizeof(lzop_magic));
	write16(&buffer, 0x1010 &0xFFFF); /* version: pretend to be LZOP version 0x1010 from lzop's version.h */
	write16(&buffer, lzo_version() & 0xFFFF); /* libversion */
	write16(&buffer, opt_filter ? 0x0950 : 0x0940); /* version needed to extract */
	write8(&buffer, M_LZO1X_1);	/* method */
	write8(&buffer, 5); /* level */
	write32(&buffer, flags); /* flags */
	/* if (flags & F_H_FILTER) 
		write32(iow, opt_filter); 
	*/ 
	write32(&buffer, 0x600); /* mode: We assume traces may be sensitive */
	write32(&buffer, time(NULL)); /* mtime */
	write32(&buffer, 0); /* GMTdiff */

	/* Length, filename */
	write8(&buffer, strlen("compresseddata"));
	write_buf(&buffer, "compresseddata",strlen("compresseddata"));

	if (flags & F_H_CRC32) {
		write32(&buffer, 
			lzo_crc32(CRC32_INIT_VALUE, 
				(const void*)buffer.buffer+sizeof(lzop_magic), 
				buffer.offset-sizeof(lzop_magic)));
	}
	else {
		uint32_t chksum=lzo_adler32(
			ADLER32_INIT_VALUE, 
			(const void *)buffer.buffer+sizeof(lzop_magic), 
			buffer.offset-sizeof(lzop_magic));
		write32(&buffer, chksum);
	}

	wandio_wwrite(DATA(iow)->child,
		buffer.buffer,
		buffer.offset);

	/* Set up the thread pool -- one thread per core */
	DATA(iow)->threads = min((uint32_t)sysconf(_SC_NPROCESSORS_ONLN),
			use_threads);
	DATA(iow)->thread = malloc(
			sizeof(struct lzothread_t) * DATA(iow)->threads);
	DATA(iow)->next_thread = 0;
	for(i=0; i<DATA(iow)->threads; ++i) {
		pthread_cond_init(&DATA(iow)->thread[i].in_ready, NULL);
		pthread_cond_init(&DATA(iow)->thread[i].out_ready, NULL);
		pthread_mutex_init(&DATA(iow)->thread[i].mutex, NULL);
		DATA(iow)->thread[i].closing = false;
		DATA(iow)->thread[i].num = i;
		DATA(iow)->thread[i].state = EMPTY;
		DATA(iow)->thread[i].inbuf.offset = 0;

		pthread_create(&DATA(iow)->thread[i].thread, 
				NULL,
				lzo_compress_thread,
				(void*)&DATA(iow)->thread[i]);
	}

	return iow;
}