 ***************************************************/

static void __parser_add_pkey_range_to_port_map(
    cl_qmap_t  * p_map,
    uint64_t  ** range_arr,
    unsigned     range_len)
{
    unsigned i;
    uint64_t pkey_64;
    ib_net16_t pkey;
    osm_prtn_t * p_prtn;

    if (!range_arr || !range_len)
        return;

    for (i = 0; i < range_len; i++) {
         for (pkey_64 = range_arr[i][0]; pkey_64 <= range_arr[i][1]; pkey_64++) {
             pkey = cl_hton16((uint16_t)(pkey_64 & 0x7fff));
             p_prtn = (osm_prtn_t *)
                 cl_qmap_get(&p_qos_policy->p_subn->prtn_pkey_tbl, pkey);
             if (p_prtn != (osm_prtn_t *)cl_qmap_end(
                   &p_qos_policy->p_subn->prtn_pkey_tbl)) {
                 __parser_add_map_to_port_map(p_map, &p_prtn->part_guid_tbl);
                 __parser_add_map_to_port_map(p_map, &p_prtn->full_guid_tbl);
             }
         }
         free(range_arr[i]);
    }