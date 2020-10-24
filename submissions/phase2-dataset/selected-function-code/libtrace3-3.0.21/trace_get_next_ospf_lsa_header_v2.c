DLLEXPORT int trace_get_next_ospf_lsa_header_v2(unsigned char **current,
                libtrace_ospf_lsa_v2_t **lsa_hdr,
                uint32_t *remaining,
                uint8_t *lsa_type,
                uint16_t *lsa_length) {

        int valid_lsa = 0;

        if (*current == NULL || *remaining < sizeof(libtrace_ospf_lsa_v2_t)) {
                *lsa_hdr = NULL;
                *remaining = 0;
                return 0;

        }

        *lsa_hdr = (libtrace_ospf_lsa_v2_t *)(*current);

        /* Check that the LSA type is valid */
        switch ((*lsa_hdr)->lsa_type) {
                case TRACE_OSPF_LS_ROUTER:
                case TRACE_OSPF_LS_NETWORK:
                case TRACE_OSPF_LS_SUMMARY:
                case TRACE_OSPF_LS_ASBR_SUMMARY:
                case TRACE_OSPF_LS_EXTERNAL:
                        valid_lsa = 1;
                        break;
        }

        /* This function is for reading LSA headers only, e.g. those in DB desc
         * or LS Ack packets. As such, I'm going to set the type and length to
         * values that should prevent anyone from trying to treat subsequent
         * payload as an LSA body */
        *lsa_type = 0;
        *lsa_length = sizeof(libtrace_ospf_lsa_v2_t);

        if (!valid_lsa) {
                *remaining = 0;
                return -1;
        }

        *remaining -= *lsa_length;
        *current += *lsa_length;

        if (remaining == 0) {
                /* No more LSAs */
                return 0;
        }
        return 1;
}