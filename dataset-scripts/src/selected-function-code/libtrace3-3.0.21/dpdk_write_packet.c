static int dpdk_write_packet(libtrace_out_t *trace, 
		libtrace_packet_t *packet){
    struct rte_mbuf* m_buff[1];
    
    int wirelen = trace_get_wire_length(packet);
    int caplen = trace_get_capture_length(packet);
    
    /* Check for a checksum and remove it */
    if (trace_get_link_type(packet) == TRACE_TYPE_ETH &&
                                            wirelen == caplen)
        caplen -= ETHER_CRC_LEN;

    m_buff[0] = rte_pktmbuf_alloc(FORMAT(trace)->pktmbuf_pool);
    if (m_buff[0] == NULL) {
        trace_set_err_out(trace, errno, "Cannot get an empty packet buffer");
        return -1;
    } else {
        int ret;
        memcpy(rte_pktmbuf_append(m_buff[0], caplen), packet->payload, caplen);
        do {
            ret = rte_eth_tx_burst(FORMAT(trace)->queue_id, FORMAT(trace)->port, m_buff, 1);
        } while (ret != 1);
    }

    return 0;
}