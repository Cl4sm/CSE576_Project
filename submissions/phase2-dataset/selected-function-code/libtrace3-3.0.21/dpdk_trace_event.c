static libtrace_eventobj_t dpdk_trace_event(libtrace_t *trace,
                                        libtrace_packet_t *packet) {
    libtrace_eventobj_t event = {0,0,0.0,0};
    int nb_rx; /* Number of receive packets we've read */
    struct rte_mbuf* pkts_burst[1]; /* Array of 1 pointer(s) to rx buffers */
    
    do {
    
        /* See if we already have a packet waiting */
        nb_rx = rte_eth_rx_burst(FORMAT(trace)->port,
                        FORMAT(trace)->queue_id, pkts_burst, 1);
        
        if (nb_rx > 0) {
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
            event.type = TRACE_EVENT_PACKET;
            event.size = dpdk_ready_pkt(trace, packet, pkts_burst[0]);
            
            /* XXX - Check this passes the filter trace_read_packet normally
             * does this for us but this wont */
            if (trace->filter) {
                if (!trace_apply_filter(trace->filter, packet)) {
                    /* Failed the filter so we loop for another packet */
                    trace->filtered_packets ++;
                    continue;
                }
            }
            trace->accepted_packets ++;
        } else {
            /* We only want to sleep for a very short time - we are non-blocking */
            event.type = TRACE_EVENT_SLEEP;
            event.seconds = 0.0001;
            event.size = 0;
        }
        
        /* If we get here we have our event */
        break;
    } while (1);

    return event;
}