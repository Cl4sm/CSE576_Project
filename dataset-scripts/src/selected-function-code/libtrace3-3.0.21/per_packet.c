static void per_packet(libtrace_packet_t *a, libtrace_packet_t *b)
{
	char *buf_a, *buf_b;
	libtrace_linktype_t lt;
	uint32_t rem_a, rem_b;


	buf_a = trace_get_packet_buffer(a, &lt, &rem_a);
	buf_b = trace_get_packet_buffer(b, &lt, &rem_b);

	if (rem_a > trace_get_wire_length(a))
		rem_a = trace_get_wire_length(a);
	if (rem_b > trace_get_wire_length(b))
		rem_b = trace_get_wire_length(b);
	

	if (!buf_a && !buf_b)
		return;

	if (!buf_a || !buf_b) {
		trace_dump_packet(a);
		trace_dump_packet(b);
		printf("****************\n");
		dumped_diff ++;
		return;
	}
		

	if (rem_a == 0 || rem_b == 0)
		return;

	if (rem_a != rem_b) {
		trace_dump_packet(a);
		trace_dump_packet(b);
		printf("****************\n");
		dumped_diff ++;
		return;
	}

	/* This is not exactly going to be snappy, but it's the easiest way
	 * to look for differences */
	if (memcmp(buf_a, buf_b, rem_a) != 0) {
		trace_dump_packet(a);
		trace_dump_packet(b);
		printf("****************\n");
		dumped_diff ++;
	}

}