static int dag_read_packet(libtrace_t *libtrace, libtrace_packet_t *packet) {
        int numbytes;
        int size = 0;
        uint32_t flags = 0;
	struct timeval tv;
        dag_record_t *erfptr = NULL;

	/* Check if we're due for a DUCK report */
        if (DUCK.last_pkt - DUCK.last_duck > DUCK.duck_freq &&
                        DUCK.duck_freq != 0) {
                size = dag_get_duckinfo(libtrace, packet);
                DUCK.last_duck = DUCK.last_pkt;
                if (size != 0) {
                        return size;
                }
                /* No DUCK support, so don't waste our time anymore */
                DUCK.duck_freq = 0;
        }

	/* Don't let anyone try to free our DAG memory hole */
	flags |= TRACE_PREP_DO_NOT_OWN_BUFFER;
	
	/* If the packet buffer is currently owned by libtrace, free it so
	 * that we can set the packet to point into the DAG memory hole */
	if (packet->buf_control == TRACE_CTRL_PACKET) {
                packet->buf_control = TRACE_CTRL_EXTERNAL;
                free(packet->buffer);
                packet->buffer = 0;
        }

	/* Grab a full ERF record */
        do {
                numbytes = dag_available(libtrace);
                if (numbytes < 0)
                        return numbytes;
                if (numbytes == 0)
                        continue;
                erfptr = dag_get_record(libtrace);
        } while (erfptr == NULL);
        
	/* Prepare the libtrace packet */
	if (dag_prepare_packet(libtrace, packet, erfptr, TRACE_RT_DATA_ERF, 
				flags))
		return -1;
	
	/* Update the DUCK timer */
	tv = trace_get_timeval(packet);
        DUCK.last_pkt = tv.tv_sec;
        return packet->payload ? htons(erfptr->rlen) : erf_get_framing_length(packet);
}