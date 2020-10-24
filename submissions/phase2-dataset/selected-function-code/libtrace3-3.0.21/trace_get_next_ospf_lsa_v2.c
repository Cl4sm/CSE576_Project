DLLEXPORT int trace_get_next_ospf_lsa_v2(unsigned char **current,
                libtrace_ospf_lsa_v2_t **lsa_hdr,
                unsigned char **lsa_body,
                uint32_t *remaining,
                uint8_t *lsa_type,
                uint16_t *lsa_length) {

        int valid_lsa = 0;

        if (*current == NULL || *remaining < sizeof(libtrace_ospf_lsa_v2_t)) {
                *lsa_hdr = NULL;
                *lsa_body = NULL;
                *remaining = 0;

                return 0;

        }

        *lsa_hdr = (libtrace_ospf_lsa_v2_t *)(*current);
        *lsa_type = (*lsa_hdr)->lsa_type;
        *lsa_length = ntohs((*lsa_hdr)->length);

        /* Check that the LSA type is valid */
        switch (*lsa_type) {
                case TRACE_OSPF_LS_ROUTER:
                case TRACE_OSPF_LS_NETWORK:
                case TRACE_OSPF_LS_SUMMARY:
                case TRACE_OSPF_LS_ASBR_SUMMARY:
                case TRACE_OSPF_LS_EXTERNAL:
                        valid_lsa = 1;
                        break;
        }

        if (*lsa_length > *remaining || !valid_lsa) {
                /* LSA is incomplete or an invalid type.
                 *
                 * If this occurs, you've probably managed to read something
                 * that is NOT a legit LSA */
                *remaining = 0;
                *lsa_body = NULL;
                return -1;
        }

        /* Some OSPF packets, e.g. LS ACKs, only contain LSA headers. If this
         * is the case, we'll set the body pointer to NULL so the caller 
         * can't read invalid data */
        if (*lsa_length == sizeof(libtrace_ospf_lsa_v2_t))
                *lsa_body = NULL;
        else
                *lsa_body = (*current + sizeof(libtrace_ospf_lsa_v2_t));

        *remaining -= *lsa_length;
        *current += *lsa_length;

        if (remaining == 0) {
                /* No more LSAs */
                return 0;
        }

        return 1;

}