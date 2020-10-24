int get_stat(void)
{
    int             i,fd, ret=-1;
    static int wait_pcks_counter=WAIT_PCKS_COUNTER+1;
    unsigned int    len;
    unsigned long   rx_o, tx_o;
    struct          nmparms params, params2;
    mib_ifEntry     *if_buf;

    static nmapi_logstat *if_ptr = (nmapi_logstat *) 0;
    if (ifdata.if_valid == 1 && (fd = open_mib("/dev/lan", O_RDWR, 0, 0)) >= 0)
    {
        if ((if_buf = (mib_ifEntry *) malloc (sizeof(mib_ifEntry))) != 0)
        {
            if_buf->ifIndex = ifdata.if_id;
            params.objid  = ID_ifEntry;
            params.buffer = if_buf;
            len = (unsigned int) sizeof(mib_ifEntry);
            params.len    = &len;
            if ((ret = get_mib_info(fd, &params)) == 0)
            {
                rx_o = stats.rx_bytes; tx_o = stats.tx_bytes;

                stats.tx_bytes = if_buf->ifOutOctets;
                stats.rx_bytes = if_buf->ifInOctets;
                stats.tx_errors = if_buf->ifOutErrors;
                stats.rx_errors = if_buf->ifInErrors;

                if (rx_o > stats.rx_bytes)
                    stats.rx_over++;
                if (tx_o > stats.tx_bytes)
                    stats.tx_over++;
            }
        }
        free(if_buf);

        /*
         * Getting the tx/rx packets every run often hurts to much performance
         * With WAIT_PCKS_COUNTER=15 i save on my system 43% cpu usage.instead of
         * WAIT_PCKS_COUNTER=0
         */
        if( wait_pcks_counter > WAIT_PCKS_COUNTER )
        {
            if ((if_ptr = (nmapi_logstat *) malloc(sizeof(nmapi_logstat) * ifdata.if_amount)) != 0 )
            {
                len = (unsigned int) ifdata.if_amount *sizeof(nmapi_logstat);
                if ((ret = get_logical_stat(if_ptr, &len)) == 0)
                {
                    for (i=0; i <= ifdata.if_amount; i++)
                    {
                        if(if_ptr[i].ifindex == ifdata.if_id)
                        {
                            stats.tx_packets = if_ptr[i].out_packets;
                            stats.rx_packets = if_ptr[i].in_packets;
                        }
                    }
                }
            }
            free(if_ptr);
            wait_pcks_counter=0;
        }
        else
        {
            wait_pcks_counter++;
        }
    }
    close_mib(fd);

    return(0);
}
