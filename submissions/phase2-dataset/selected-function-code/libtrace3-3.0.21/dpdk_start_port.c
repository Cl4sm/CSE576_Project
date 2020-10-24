static int dpdk_start_port (struct dpdk_format_data_t * format_data, char *err, int errlen){
    int ret; /* Check return values for errors */
    struct rte_eth_link link_info; /* Wait for link */
    
    /* Already started */
    if (format_data->paused == DPDK_RUNNING)
        return 0;

    /* First time started we need to alloc our memory, doing this here 
     * rather than in environment setup because we don't have snaplen then */
    if (format_data->paused == DPDK_NEVER_STARTED) {
        if (format_data->snaplen == 0) {
            format_data->snaplen = RX_MBUF_SIZE;
            port_conf.rxmode.jumbo_frame = 0;
            port_conf.rxmode.max_rx_pkt_len = 0;
        } else {
            /* Use jumbo frames */
            port_conf.rxmode.jumbo_frame = 1;
            port_conf.rxmode.max_rx_pkt_len = format_data->snaplen;
        }

        /* This is additional overhead so make sure we allow space for this */
#if GET_MAC_CRC_CHECKSUM
        format_data->snaplen += ETHER_CRC_LEN;
#endif
#if HAS_HW_TIMESTAMPS_82580
        format_data->snaplen += sizeof(struct hw_timestamp_82580);
#endif

        /* Create the mbuf pool, which is the place our packets are allocated
         * from - TODO figure out if there is is a free function (I cannot see one) 
         * NOTE: RX queue requires nb_packets + 1 otherwise it fails to 
         * allocate however that extra 1 packet is not used. 
         * (I assume <= vs < error some where in DPDK code)
         * TX requires nb_tx_buffers + 1 in the case the queue is full 
         * so that will fill the new buffer and wait until slots in the
         * ring become available.
         */
#if DEBUG
    fprintf(stderr, "Creating mempool named %s\n", format_data->mempool_name);
#endif
        format_data->pktmbuf_pool =
            rte_mempool_create(format_data->mempool_name,
                       format_data->nb_rx_buf + format_data->nb_tx_buf + 1,
                       format_data->snaplen + sizeof(struct rte_mbuf) 
                                        + RTE_PKTMBUF_HEADROOM,
                       8, sizeof(struct rte_pktmbuf_pool_private),
                       rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL,
                       0, MEMPOOL_F_SP_PUT | MEMPOOL_F_SC_GET);

        if (format_data->pktmbuf_pool == NULL) {
            snprintf(err, errlen, "Intel DPDK - Initialisation of mbuf "
                        "pool failed: %s", strerror(rte_errno));
            return -1;
        }
    }
    
    /* ----------- Now do the setup for the port mapping ------------ */
    /* Order of calls must be 
     * rte_eth_dev_configure()
     * rte_eth_tx_queue_setup()
     * rte_eth_rx_queue_setup()
     * rte_eth_dev_start()
     * other rte_eth calls
     */
    
    /* This must be called first before another *eth* function
     * 1 rx, 1 tx queue, port_conf sets checksum stripping etc */
    ret = rte_eth_dev_configure(format_data->port, 1, 1, &port_conf);
    if (ret < 0) {
        snprintf(err, errlen, "Intel DPDK - Cannot configure device port"
                            " %"PRIu8" : %s", format_data->port,
                            strerror(-ret));
        return -1;
    }
    /* Initialise the TX queue a minimum value if using this port for
     * receiving. Otherwise a larger size if writing packets.
     */
    ret = rte_eth_tx_queue_setup(format_data->port, format_data->queue_id,
                        format_data->nb_tx_buf, SOCKET_ID_ANY, &tx_conf);
    if (ret < 0) {
        snprintf(err, errlen, "Intel DPDK - Cannot configure TX queue on port"
                            " %"PRIu8" : %s", format_data->port,
                            strerror(-ret));
        return -1;
    }
    /* Initialise the RX queue with some packets from memory */
    ret = rte_eth_rx_queue_setup(format_data->port, format_data->queue_id,
                            format_data->nb_rx_buf, SOCKET_ID_ANY, 
                            &rx_conf, format_data->pktmbuf_pool);
    if (ret < 0) {
        snprintf(err, errlen, "Intel DPDK - Cannot configure RX queue on port"
                    " %"PRIu8" : %s", format_data->port,
                    strerror(-ret));
        return -1;
    }
    
    /* Start device */
    ret = rte_eth_dev_start(format_data->port);
    if (ret < 0) {
        snprintf(err, errlen, "Intel DPDK - rte_eth_dev_start failed : %s",
                    strerror(-ret));
        return -1;
    }

    /* Default promiscuous to on */
    if (format_data->promisc == -1)
        format_data->promisc = 1;
    
    if (format_data->promisc == 1)
        rte_eth_promiscuous_enable(format_data->port);
    else
        rte_eth_promiscuous_disable(format_data->port);
    
    /* Wait for the link to come up */
    rte_eth_link_get(format_data->port, &link_info);
#if DEBUG
    fprintf(stderr, "Link status is %d %d %d\n", (int) link_info.link_status,
            (int) link_info.link_duplex, (int) link_info.link_speed);
#endif

    /* We have now successfully started/unpaused */
    format_data->paused = DPDK_RUNNING;
    
    return 0;
}