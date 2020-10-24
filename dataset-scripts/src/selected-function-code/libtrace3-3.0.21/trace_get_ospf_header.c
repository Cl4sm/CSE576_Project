DLLEXPORT void *trace_get_ospf_header(libtrace_packet_t *packet,
                uint8_t *version, uint32_t *remaining) {
        uint8_t proto;
        void *ospf;
        uint32_t dummy_rem = 0;


        if (!remaining)
                remaining = &dummy_rem;

        assert(version != NULL && "version may not be NULL when calling trace_get_ospf_header!");

        ospf = trace_get_transport(packet, &proto, remaining);

        if (!ospf || proto != TRACE_IPPROTO_OSPF || *remaining == 0)
                return NULL;

        *version = *((uint8_t *)ospf);

        if (*version == 2 && *remaining < sizeof(libtrace_ospf_v2_t))
                return NULL;

        return ospf;
}