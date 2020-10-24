static int dag_get_duckinfo(libtrace_t *libtrace,
                                libtrace_packet_t *packet) {
        dag_inf lt_dag_inf;

	/* Allocate memory for the DUCK data */
        if (packet->buf_control == TRACE_CTRL_EXTERNAL ||
                        !packet->buffer) {
                packet->buffer = malloc(LIBTRACE_PACKET_BUFSIZE);
                packet->buf_control = TRACE_CTRL_PACKET;
                if (!packet->buffer) {
                        trace_set_err(libtrace, errno,
                                        "Cannot allocate packet buffer");
                        return -1;
                }
        }

	/* DUCK doesn't actually have a format header, as such */
        packet->header = 0;
        packet->payload = packet->buffer;

	/* Check that the DAG card supports DUCK */
        if ((ioctl(FORMAT_DATA->fd, DAG_IOINF, &lt_dag_inf) < 0)) {
                trace_set_err(libtrace, errno,
                                "Error using DAG_IOINF");
                return -1;
        }
        if (!IsDUCK(&lt_dag_inf)) {
                printf("WARNING: %s does not have modern clock support - No DUCK information will be gathered\n", libtrace->uridata);
                return 0;
        }

	/* Get the DUCK information from the card */
        if ((ioctl(FORMAT_DATA->fd, DAG_IOGETDUCK, (duck_inf *)packet->payload)
                                < 0)) {
                trace_set_err(libtrace, errno, "Error using DAG_IOGETDUCK");
                return -1;
        }

	/* Set the type */
        packet->type = TRACE_RT_DUCK_2_4;

	/* Set the packet's trace to point at a DUCK trace, so that the
	 * DUCK format functions will be called on the packet rather than the
	 * DAG ones */
        if (!DUCK.dummy_duck)
                DUCK.dummy_duck = trace_create_dead("duck:dummy");
        packet->trace = DUCK.dummy_duck;
        return sizeof(duck_inf);
}