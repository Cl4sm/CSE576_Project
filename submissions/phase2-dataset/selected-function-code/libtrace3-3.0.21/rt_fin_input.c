static int rt_fin_input(libtrace_t *libtrace) {
	/* Make sure we clean up any dummy traces that we have been using */
	
	if (RT_INFO->dummy_duck)
		trace_destroy_dead(RT_INFO->dummy_duck);

	if (RT_INFO->dummy_erf) 
		trace_destroy_dead(RT_INFO->dummy_erf);
		
	if (RT_INFO->dummy_pcap)
		trace_destroy_dead(RT_INFO->dummy_pcap);

	if (RT_INFO->dummy_linux)
		trace_destroy_dead(RT_INFO->dummy_linux);
	
	if (RT_INFO->dummy_ring)
		trace_destroy_dead(RT_INFO->dummy_ring);

	if (RT_INFO->dummy_bpf)
		trace_destroy_dead(RT_INFO->dummy_bpf);
	free(libtrace->format_data);
        return 0;
}