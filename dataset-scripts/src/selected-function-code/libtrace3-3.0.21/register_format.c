void register_format(struct libtrace_format_t *f) {
	assert(f->next==NULL); /* Can't register a format twice */
	f->next=formats_list;
	formats_list=f;

	/* Now, verify that the format has at least the minimum functionality.
	 * 
	 * This #if can be changed to a 1 to output warnings about inconsistent
	 * functions being provided by format modules.  This generally is very
	 * noisy, as almost all modules don't implement one or more functions
	 * for various reasons.  This is very useful when checking a new 
	 * format module is sane.
	 */ 
#if 0
	if (f->init_input) {
#define REQUIRE(x) \
		if (!f->x) \
			fprintf(stderr,"%s: Input format should provide " #x "\n",f->name)
		REQUIRE(read_packet);
		REQUIRE(start_input);
		REQUIRE(fin_input);
		REQUIRE(get_link_type);
		REQUIRE(get_capture_length);
		REQUIRE(get_wire_length);
		REQUIRE(get_framing_length);
		REQUIRE(trace_event);
		if (!f->get_erf_timestamp 
			&& !f->get_seconds
			&& !f->get_timeval) {
			fprintf(stderr,"%s: A trace format capable of input, should provide at least one of\n"
"get_erf_timestamp, get_seconds or trace_timeval\n",f->name);
		}
		if (f->trace_event!=trace_event_trace) {
			/* Theres nothing that a trace file could optimise with
			 * config_input
			 */
			REQUIRE(pause_input);
			REQUIRE(config_input);
			REQUIRE(get_fd);
		}
		else {
			if (f->get_fd) {
				fprintf(stderr,"%s: Unnecessary get_fd\n",
						f->name);
			}
		}
#undef REQUIRE
	}
	else {
#define REQUIRE(x) \
		if (f->x) \
			fprintf(stderr,"%s: Non Input format shouldn't need " #x "\n",f->name)
		REQUIRE(read_packet);
		REQUIRE(start_input);
		REQUIRE(pause_input);
		REQUIRE(fin_input);
		REQUIRE(get_link_type);
		REQUIRE(get_capture_length);
		REQUIRE(get_wire_length);
		REQUIRE(get_framing_length);
		REQUIRE(trace_event);
		REQUIRE(get_seconds);
		REQUIRE(get_timeval);
		REQUIRE(get_erf_timestamp);
#undef REQUIRE
	}
	if (f->init_output) {
#define REQUIRE(x) \
		if (!f->x) \
			fprintf(stderr,"%s: Output format should provide " #x "\n",f->name)
		REQUIRE(write_packet);
		REQUIRE(start_output);
		REQUIRE(config_output);
		REQUIRE(fin_output);
#undef REQUIRE
	}
	else {
#define REQUIRE(x) \
		if (f->x) \
			fprintf(stderr,"%s: Non Output format shouldn't need " #x "\n",f->name)
		REQUIRE(write_packet);
		REQUIRE(start_output);
		REQUIRE(config_output);
		REQUIRE(fin_output);
#undef REQUIRE
	}
#endif
}