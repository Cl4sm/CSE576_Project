DLLEXPORT void *trace_get_ospf_contents_v2(libtrace_ospf_v2_t *header,
                uint8_t *ospf_type, uint32_t *remaining) {

        uint8_t dummy_type;
        char *ptr;

        assert(remaining != NULL && "remaining may not be NULL when calling trace_get_ospf_contents!");

        if (!ospf_type)
                ospf_type = &dummy_type;

        if (!header || *remaining < sizeof(libtrace_ospf_v2_t)) {
                *ospf_type = 0;
                *remaining = 0;
                return NULL;
        }

        *ospf_type = header->type;

        ptr = ((char *)header) + sizeof(libtrace_ospf_v2_t);
        *remaining -= sizeof(libtrace_ospf_v2_t);

        return (void *)ptr;

}