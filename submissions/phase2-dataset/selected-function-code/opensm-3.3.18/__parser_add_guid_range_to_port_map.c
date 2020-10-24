 ***************************************************/

static void __parser_add_guid_range_to_port_map(
    cl_qmap_t  * p_map,
    uint64_t  ** range_arr,
    unsigned     range_len)
{
    unsigned i;
    uint64_t guid_ho;
    osm_port_t * p_osm_port;

    if (!range_arr || !range_len)
        return;

    for (i = 0; i < range_len; i++) {
         for (guid_ho = range_arr[i][0]; guid_ho <= range_arr[i][1]; guid_ho++) {
             p_osm_port =
                osm_get_port_by_guid(p_qos_policy->p_subn, cl_hton64(guid_ho));
             if (p_osm_port)
                 __parser_add_port_to_port_map(p_map, p_osm_port->p_physp);
         }
         free(range_arr[i]);
    }