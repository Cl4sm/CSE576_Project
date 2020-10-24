 ***************************************************/

static void __parser_add_partition_list_to_port_map(
    cl_qmap_t  * p_map,
    cl_list_t  * p_list)
{
    cl_list_iterator_t    list_iterator;
    char                * tmp_str;
    osm_prtn_t          * p_prtn;

    /* extract all the ports from the partition
       to the port map of this port group */
    list_iterator = cl_list_head(p_list);
    while(list_iterator != cl_list_end(p_list)) {
        tmp_str = (char*)cl_list_obj(list_iterator);
        if (tmp_str) {
            p_prtn = osm_prtn_find_by_name(p_qos_policy->p_subn, tmp_str);
            if (p_prtn) {
                __parser_add_map_to_port_map(p_map, &p_prtn->part_guid_tbl);
                __parser_add_map_to_port_map(p_map, &p_prtn->full_guid_tbl);
            }
            free(tmp_str);
        }
        list_iterator = cl_list_next(list_iterator);
    }