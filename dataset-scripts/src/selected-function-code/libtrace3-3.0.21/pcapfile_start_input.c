static int pcapfile_start_input(libtrace_t *libtrace) 
{
	int err;

	if (!libtrace->io) {
		libtrace->io=trace_open_file(libtrace);
		DATA(libtrace)->started=false;
	}

	if (!DATA(libtrace)->started) {

		if (!libtrace->io)
			return -1;

		err=wandio_read(libtrace->io,
				&DATA(libtrace)->header,
				sizeof(DATA(libtrace)->header));

		DATA(libtrace)->started = true;
		assert(sizeof(DATA(libtrace)->header) > 0);
		
		if (err<1) {
			if (err == 0) {
				trace_set_err(libtrace, TRACE_ERR_INIT_FAILED,
						"Reading pcap file header\n");
			}
			return -1;
		}
		
		if (!header_is_magic(&(DATA(libtrace)->header))) {
			trace_set_err(libtrace,TRACE_ERR_INIT_FAILED,
					"Not a pcap tracefile (magic=%08x)\n",swapl(libtrace,DATA(libtrace)->header.magic_number));
			return -1; /* Not a pcap file */
		}

		if (swaps(libtrace,DATA(libtrace)->header.version_major)!=2
			&& swaps(libtrace,DATA(libtrace)->header.version_minor)!=4) {
			trace_set_err(libtrace,TRACE_ERR_INIT_FAILED,
					"Unknown pcap tracefile version %d.%d\n",
					swaps(libtrace,
						DATA(libtrace)->header.version_major),
					swaps(libtrace,
						DATA(libtrace)->header.version_minor));
			return -1;
		}

	}

	return 0;
}