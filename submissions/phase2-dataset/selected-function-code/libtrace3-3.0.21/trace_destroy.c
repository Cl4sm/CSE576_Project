DLLEXPORT void trace_destroy(libtrace_t *libtrace) {
        assert(libtrace);
	if (libtrace->format) {
		if (libtrace->started && libtrace->format->pause_input)
			libtrace->format->pause_input(libtrace);
		if (libtrace->format->fin_input)
			libtrace->format->fin_input(libtrace);
	}
        /* Need to free things! */
        if (libtrace->uridata)
		free(libtrace->uridata);
	if (libtrace->event.packet) {
		/* Don't use trace_destroy_packet here - there is almost
		 * certainly going to be another libtrace_packet_t that is
		 * pointing to the buffer for this packet, so we don't want
		 * to free it. Rather, it will get freed when the user calls
		 * trace_destroy_packet on the libtrace_packet_t that they
		 * own.
		 *
		 * All we need to do then is free our packet structure itself.
		 */
		 free(libtrace->event.packet);
	}
        free(libtrace);
}