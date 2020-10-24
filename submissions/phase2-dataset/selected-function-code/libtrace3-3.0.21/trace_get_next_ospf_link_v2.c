DLLEXPORT int trace_get_next_ospf_link_v2(unsigned char **current,
                libtrace_ospf_link_v2_t **link,
                uint32_t *remaining,
                uint32_t *link_len) {

        if (*current == NULL || *remaining < sizeof(libtrace_ospf_link_v2_t)) {
                *remaining = 0;
                *link = NULL;
                return 0;
        }

        *link = (libtrace_ospf_link_v2_t *)*current;

        /* XXX The spec allows for multiple metrics for a single link. This
         * approach won't support this, so we may need to be more intelligent
         * about this in future */
        *remaining -= sizeof(libtrace_ospf_link_v2_t);
        *link_len = sizeof(libtrace_ospf_link_v2_t);
        *current += sizeof(libtrace_ospf_link_v2_t);

        return 1;

}