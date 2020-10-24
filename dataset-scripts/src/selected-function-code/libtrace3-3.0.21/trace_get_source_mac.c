DLLEXPORT uint8_t *trace_get_source_mac(libtrace_packet_t *packet) {
        void *link;
        uint32_t remaining;
        libtrace_linktype_t linktype;
        assert(packet);
        link = trace_get_layer2(packet,&linktype,&remaining);

        if (!link)
                return NULL;

        switch (linktype) {
                case TRACE_TYPE_ETH:
                        return (uint8_t *)&(((libtrace_ether_t*)link)->ether_shost);
                case TRACE_TYPE_80211:
                        return get_source_mac_from_wifi(link);
                /* These packets don't have MAC addresses */
                case TRACE_TYPE_POS:
                case TRACE_TYPE_NONE:
                case TRACE_TYPE_HDLC_POS:
                case TRACE_TYPE_PFLOG:
                case TRACE_TYPE_ATM:
                case TRACE_TYPE_DUCK:
                case TRACE_TYPE_METADATA:
                case TRACE_TYPE_AAL5:
                case TRACE_TYPE_LLCSNAP:
                case TRACE_TYPE_PPP:
		case TRACE_TYPE_NONDATA:
		case TRACE_TYPE_OPENBSD_LOOP:
		case TRACE_TYPE_UNKNOWN:
                        return NULL;

                /* Metadata headers should already be skipped */
                case TRACE_TYPE_LINUX_SLL:
                case TRACE_TYPE_80211_PRISM:
                case TRACE_TYPE_80211_RADIO:
                        assert(!"Metadata headers should already be skipped");
                        break;
        }
        fprintf(stderr,"%s not implemented for linktype %i\n", __func__, linktype);
        assert(0);
        return NULL;
}