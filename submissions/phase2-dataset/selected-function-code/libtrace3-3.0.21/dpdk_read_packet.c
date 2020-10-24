static int dpdk_read_packet (libtrace_t *libtrace, libtrace_packet_t *packet) {
    int nb_rx; /* Number of rx packets we've recevied */
    struct rte_mbuf* pkts_burst[1]; /* Array of 1 pointer(s) */

    /* Free the last packet buffer */
    if (packet->buffer != NULL) {
        /* Buffer is owned by DPDK */
        if ( packet->buf_control == TRACE_CTRL_EXTERNAL ) {
            rte_pktmbuf_free(packet->buffer);
            packet->buffer = NULL;
        } else
        /* Buffer is owned by packet i.e. has been malloc'd */
        if (packet->buf_control == TRACE_CTRL_PACKET) {
            free(packet->buffer);
            packet->buffer = NULL;
        }
    }
    
    packet->buf_control = TRACE_CTRL_EXTERNAL;
    packet->type = TRACE_RT_DATA_DPDK;
    
    /* Wait for a packet */
    while (1) {
        /* Poll for a single packet */
        nb_rx = rte_eth_rx_burst(FORMAT(libtrace)->port,
                            FORMAT(libtrace)->queue_id, pkts_burst, 1);        
        if (nb_rx > 0) { /* Got a packet - otherwise we keep spining */
            return dpdk_ready_pkt(libtrace, packet, pkts_burst[0]);
        }
    }
    
    /* We'll never get here - but if we did it would be bad */
    return -1;
}