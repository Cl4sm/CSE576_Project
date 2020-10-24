static int dpdk_init_output(libtrace_out_t *libtrace)
{
    char err[500];
    err[0] = 0;
    
    libtrace->format_data = (struct dpdk_format_data_t *)
                            malloc(sizeof(struct dpdk_format_data_t));
    FORMAT(libtrace)->port = 0; /* Always assume 1 port loaded */
    FORMAT(libtrace)->queue_id = 0; /* Single queue per port */
    FORMAT(libtrace)->nb_ports = 0;
    FORMAT(libtrace)->snaplen = 0; /* Use default */
    FORMAT(libtrace)->nb_rx_buf = MIN_NB_BUF;
    FORMAT(libtrace)->nb_tx_buf = NB_TX_MBUF;
    FORMAT(libtrace)->promisc = -1;
    FORMAT(libtrace)->pktmbuf_pool = NULL;
#if DPDK_USE_BLACKLIST
    FORMAT(libtrace)->nb_blacklist = 0;
#endif
    FORMAT(libtrace)->paused = DPDK_NEVER_STARTED;
    FORMAT(libtrace)->mempool_name[0] = 0;
#if HAS_HW_TIMESTAMPS_82580
    FORMAT(libtrace)->ts_first_sys = 0;
    FORMAT(libtrace)->ts_last_sys = 0;
    FORMAT(libtrace)->wrap_count = 0;
#endif

    if (dpdk_init_environment(libtrace->uridata, FORMAT(libtrace), err, sizeof(err)) != 0) {
        trace_set_err_out(libtrace, TRACE_ERR_INIT_FAILED, "%s", err);
        free(libtrace->format_data);
        libtrace->format_data = NULL;
        return -1;
    }
    return 0;
};