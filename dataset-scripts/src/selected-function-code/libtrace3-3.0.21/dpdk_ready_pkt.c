static inline int dpdk_ready_pkt(libtrace_t *libtrace, libtrace_packet_t *packet,
                                                        struct rte_mbuf* pkt){
    uint8_t * hdr_size;
    struct dpdk_addt_hdr *hdr;
#if HAS_HW_TIMESTAMPS_82580
    struct hw_timestamp_82580 *hw_ts;
    struct timeval cur_sys_time;
    uint64_t cur_sys_time_ns;
    uint64_t estimated_wraps;
    
    /* Using gettimeofday because it's most likely to be a vsyscall
     * We don't want to slow down anything with systemcalls we dont need
     * accauracy */
    gettimeofday(&cur_sys_time, NULL);
#else
# if USE_CLOCK_GETTIME
    struct timespec cur_sys_time;
    
    /* This looks terrible and I feel bad doing it. But it's OK 
     * on new kernels, because this is a vsyscall */
    clock_gettime(CLOCK_REALTIME, &cur_sys_time);
# else
    struct timeval cur_sys_time;
    /* Should be a vsyscall */
    gettimeofday(&cur_sys_time, NULL);
# endif
#endif

    /* Record the size of our header */
    hdr_size = (uint8_t *) rte_pktmbuf_prepend(pkt, sizeof(uint8_t));
    *hdr_size = sizeof(struct dpdk_addt_hdr);
    /* Now put our header in front of that size */
    hdr = (struct dpdk_addt_hdr *) rte_pktmbuf_prepend(pkt, sizeof(struct dpdk_addt_hdr));
    memset(hdr, 0, sizeof(struct dpdk_addt_hdr));
    
#if GET_MAC_CRC_CHECKSUM
    /* Add back in the CRC sum */
    pkt->pkt.pkt_len += ETHER_CRC_LEN;
    pkt->pkt.data_len += ETHER_CRC_LEN;
    hdr->flags |= INCLUDES_CHECKSUM;
#endif

#if HAS_HW_TIMESTAMPS_82580
    /* Taken from igb_ptp.c part of Intel Linux drivers (Good example code)
     *
     *        +----------+---+   +--------------+
     *  82580 |    24    | 8 |   |      32      |
     *        +----------+---+   +--------------+
     *          reserved  \______ 40 bits _____/
     * 
     * The 40 bit 82580 SYSTIM overflows every
     *   2^40 * 10^-9 /  60  = 18.3 minutes.
     * 
     * NOTE picture is in Big Endian order, in memory it's acutally in Little
     * Endian (for the full 64 bits) i.e. picture is mirrored
     */
    
    /* The timestamp is sitting before our packet and is included in pkt_len */
    hdr->flags |= INCLUDES_HW_TIMESTAMP;
    hw_ts = (struct hw_timestamp_82580 *) MBUF_PKTDATA(pkt);
    
    /* Despite what the documentation says this is in Little 
     * Endian byteorder. Mask the reserved section out.
     */
    hdr->timestamp = le64toh(hw_ts->timestamp) & 
                ~(((~0ull)>>TS_NBITS_82580)<<TS_NBITS_82580);
                
    cur_sys_time_ns = TV_TO_NS(cur_sys_time);
    if (unlikely(FORMAT(libtrace)->ts_first_sys == 0)) {
        FORMAT(libtrace)->ts_first_sys = cur_sys_time_ns - hdr->timestamp;
        FORMAT(libtrace)->ts_last_sys = FORMAT(libtrace)->ts_first_sys;
    }
    
    /* This will have serious problems if packets aren't read quickly
     * that is within a couple of seconds because our clock cycles every
     * 18 seconds */
    estimated_wraps = (cur_sys_time_ns - FORMAT(libtrace)->ts_last_sys)
                            / (1ull<<TS_NBITS_82580);
    
    /* Estimated_wraps gives the number of times the counter should have
     * wrapped (however depending on value last time it could have wrapped
     * twice more (if hw clock is close to its max value) or once less (allowing
     * for a bit of variance between hw and sys clock). But if the clock
     * shouldn't have wrapped once then don't allow it to go backwards in time */
    if (unlikely(estimated_wraps >= 2)) {
        /* 2 or more wrap arounds add all but the very last wrap */
        FORMAT(libtrace)->wrap_count += estimated_wraps - 1;
    }
    
    /* Set the timestamp to the lowest possible value we're considering */
    hdr->timestamp += FORMAT(libtrace)->ts_first_sys +
                        FORMAT(libtrace)->wrap_count * (1ull<<TS_NBITS_82580);
    
    /* In most runs only the first if() will need evaluating - i.e our
     * estimate is correct. */
    if (unlikely(!WITHIN_VARIANCE(cur_sys_time_ns,
                                hdr->timestamp, MAXSKEW_82580))) {
        /* Failed to match estimated_wraps-1 (or estimated_wraps in ==0 case) */
        FORMAT(libtrace)->wrap_count++;
        hdr->timestamp += (1ull<<TS_NBITS_82580);
        if (!WITHIN_VARIANCE(cur_sys_time_ns,
                                hdr->timestamp, MAXSKEW_82580)) {
            /* Failed to match estimated_wraps */
            FORMAT(libtrace)->wrap_count++;
            hdr->timestamp += (1ull<<TS_NBITS_82580);
            if (!WITHIN_VARIANCE(cur_sys_time_ns,
                                hdr->timestamp, MAXSKEW_82580)) {
                if (estimated_wraps == 0) {
                    /* 0 case Failed to match estimated_wraps+2 */
                    printf("WARNING - Hardware Timestamp failed to"
                                            " match using systemtime!\n");
                    hdr->timestamp = cur_sys_time_ns;
                } else {
                    /* Failed to match estimated_wraps+1 */
                    FORMAT(libtrace)->wrap_count++;
                    hdr->timestamp += (1ull<<TS_NBITS_82580);
                    if (!WITHIN_VARIANCE(cur_sys_time_ns,
                                hdr->timestamp, MAXSKEW_82580)) {
                        /* Failed to match estimated_wraps+2 */
                        printf("WARNING - Hardware Timestamp failed to"
                                            " match using systemtime!!\n");
                    }
                }
            }
        }
    }

    /* Log our previous for the next loop */
    FORMAT(libtrace)->ts_last_sys = TV_TO_NS(cur_sys_time);

#else
# if USE_CLOCK_GETTIME
    hdr->timestamp = TS_TO_NS(cur_sys_time);
# else
    hdr->timestamp = TV_TO_NS(cur_sys_time);
# endif
#endif

    /* Intels samples prefetch into level 0 cache lets assume it is a good
     * idea and do the same */
    rte_prefetch0(rte_pktmbuf_mtod(pkt, void *));
    packet->buffer = pkt;
    dpdk_prepare_packet(libtrace, packet, packet->buffer, packet->type, 0);

    /* Set our capture length for the first time */
    hdr->cap_len = dpdk_get_wire_length(packet);
    if (!(hdr->flags & INCLUDES_CHECKSUM)) {
        hdr->cap_len -= ETHER_CRC_LEN;
    }
    

    return dpdk_get_framing_length(packet) +
                        dpdk_get_capture_length(packet);
}