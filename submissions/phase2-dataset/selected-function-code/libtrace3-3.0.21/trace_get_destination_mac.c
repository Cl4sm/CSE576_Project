DLLEXPORT uint8_t *trace_get_destination_mac(libtrace_packet_t *packet)
{
        void *link;
        libtrace_linktype_t linktype;
        uint32_t remaining;
        libtrace_80211_t *wifi;
        libtrace_ether_t *ethptr;

        link = trace_get_layer2(packet,&linktype,&remaining);

        ethptr = (libtrace_ether_t*)link;


        if (!link)
                return NULL;

        switch (linktype) {
                case TRACE_TYPE_80211:
                        wifi=(libtrace_80211_t*)link;
                        return (uint8_t*)&wifi->mac1;
                case TRACE_TYPE_ETH:
                        return (uint8_t*)&ethptr->ether_dhost;
                case TRACE_TYPE_POS:
                case TRACE_TYPE_NONE:
                case TRACE_TYPE_ATM:
                case TRACE_TYPE_HDLC_POS:
                case TRACE_TYPE_PFLOG:
                case TRACE_TYPE_DUCK:
                case TRACE_TYPE_METADATA:
		case TRACE_TYPE_AAL5:
		case TRACE_TYPE_LLCSNAP:
		case TRACE_TYPE_PPP:	
		case TRACE_TYPE_NONDATA:
		case TRACE_TYPE_OPENBSD_LOOP:
		case TRACE_TYPE_UNKNOWN:
                        /* No MAC address */
                        return NULL;
                /* Metadata headers should already be skipped */
                case TRACE_TYPE_LINUX_SLL:
                case TRACE_TYPE_80211_PRISM:
                case TRACE_TYPE_80211_RADIO:
                        assert(!"Metadata headers should already be skipped");
                        break;
        }
        fprintf(stderr,"Not implemented\n");
        assert(0);
        return NULL;
}