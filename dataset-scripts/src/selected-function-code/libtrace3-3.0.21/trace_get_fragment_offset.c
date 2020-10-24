DLLEXPORT uint16_t trace_get_fragment_offset(const libtrace_packet_t *packet, 
                uint8_t *more) {

        void *l3;
        uint16_t ethertype;
        uint32_t remaining;

        *more = 0;

        l3 = trace_get_layer3(packet, &ethertype, &remaining);
        if (l3 == NULL)
                return 0;

        if (ethertype == TRACE_ETHERTYPE_IP) {
                libtrace_ip_t *ip = (libtrace_ip_t *)l3;
                uint16_t offset = 0;

                /* Fragment offset appears in 7th and 8th bytes */
                if (remaining < 8)
                        return 0;
                 
                offset = ntohs(ip->ip_off);

                if ((offset & 0x2000) != 0)
                        *more = 1;
                return (offset & 0x1FFF) * 8;
        }

        if (ethertype == TRACE_ETHERTYPE_IPV6) {
                libtrace_ip6_t *ip6 = (libtrace_ip6_t *)l3;
                void *payload = ip6++;
                uint8_t nxt = ip6->nxt;
                uint16_t len;
                
                /* First task, find a Fragment header if present */
                if (remaining < sizeof(libtrace_ip6_t))
                        return 0;
                remaining -= sizeof(libtrace_ip6_t);

                /* Adapted from trace_get_payload_from_ip6 */
                while (1) {
                        switch (nxt) {
                        case 0:
                        case TRACE_IPPROTO_ROUTING:
			case TRACE_IPPROTO_AH: 
			case TRACE_IPPROTO_DSTOPTS: 
                        {

				/* Length does not include the first 8 bytes */
				len=((libtrace_ip6_ext_t*)payload)->len * 8;
				len += 8;

			        if (remaining < len) {
                                        /* Snap too short */
                                        return 0;
                                }
                                remaining-=len;

				nxt=((libtrace_ip6_ext_t*)payload)->nxt;
				continue;
			}
			case TRACE_IPPROTO_FRAGMENT:
                        {
                                libtrace_ip6_frag_t *frag = (libtrace_ip6_frag_t *)payload;
                                uint16_t offset;
                                len = sizeof(libtrace_ip6_frag_t);
                                if (remaining < len) {
                                        /* Snap too short */
                                        return 0;
                                }
                                remaining-=len;

                                offset = ntohs(frag->frag_off);
                                if ((offset & 0x0001) != 0) 
                                        *more = 1;

                                return ((offset & 0xFFF8) >> 3) * 8;
                         }
                         default:
                                return 0;
                         }
                }

        }
        return 0;
}