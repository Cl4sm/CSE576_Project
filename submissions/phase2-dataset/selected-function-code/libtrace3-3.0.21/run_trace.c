static void run_trace(char *uri) 
{
	struct libtrace_packet_t *packet = trace_create_packet();
	int i;
	uint64_t count = 0;
	uint64_t bytes = 0;
	uint64_t packets;

	fprintf(stderr,"%s:\n",uri);

        trace = trace_create(uri);

	if (trace_is_err(trace)) {
		trace_perror(trace,"Failed to create trace");
		return;
	}

	if (trace_start(trace)==-1) {
		trace_perror(trace,"Failed to start trace");
		return;
	}


        for (;;) {
		int psize;
		int wlen;
                if ((psize = trace_read_packet(trace, packet)) <1) {
                        break;
                }
		
		if (done)
			break;
		wlen = trace_get_wire_length(packet);

		for(i=0;i<filter_count;++i) {
			if (filters[i].filter == NULL)
				continue;
			if(trace_apply_filter(filters[i].filter,packet) > 0) {
				++filters[i].count;
				filters[i].bytes+=wlen;
			}
			if (trace_is_err(trace)) {
				trace_perror(trace, "trace_apply_filter");
				fprintf(stderr, "Removing filter from filterlist\n");
				filters[i].filter = NULL;
			}
		}

		++count;
		bytes+=wlen;
        }

	printf("%-30s\t%12s\t%12s\t%7s\n","filter","count","bytes","%");
	for(i=0;i<filter_count;++i) {
		printf("%30s:\t%12"PRIu64"\t%12"PRIu64"\t%7.03f\n",filters[i].expr,filters[i].count,filters[i].bytes,filters[i].count*100.0/count);
		filters[i].bytes=0;
		filters[i].count=0;
	}
	packets=trace_get_received_packets(trace);
	if (packets!=UINT64_MAX)
		fprintf(stderr,"%30s:\t%12" PRIu64"\n", 
				"Input packets", packets);
	packets=trace_get_filtered_packets(trace);
	if (packets!=UINT64_MAX)
		fprintf(stderr,"%30s:\t%12" PRIu64"\n", 
				"Filtered packets", packets);
	packets=trace_get_dropped_packets(trace);
	if (packets!=UINT64_MAX)
		fprintf(stderr,"%30s:\t%12" PRIu64"\n",
				"Dropped packets",packets);
	packets=trace_get_accepted_packets(trace);
	if (packets!=UINT64_MAX)
		fprintf(stderr,"%30s:\t%12" PRIu64 "\n",
				"Accepted Packets",packets);
	printf("%30s:\t%12"PRIu64"\t%12" PRIu64 "\n","Total",count,bytes);
	totcount+=count;
	totbytes+=bytes;

	if (trace_is_err(trace))
		trace_perror(trace,"%s",uri);

        trace_destroy(trace);
}