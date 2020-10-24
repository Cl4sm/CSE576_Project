 ***************************************************/

static int __validate_pkeys( uint64_t ** range_arr,
                             unsigned    range_len,
                             boolean_t   is_ipoib)
{
    unsigned i;
    uint64_t pkey_64;
    ib_net16_t pkey;
    osm_prtn_t * p_prtn;

    if (!range_arr || !range_len)
        return 0;

    for (i = 0; i < range_len; i++) {
        for (pkey_64 = range_arr[i][0]; pkey_64 <= range_arr[i][1]; pkey_64++) {
            pkey = cl_hton16((uint16_t)(pkey_64 & 0x7fff));
            p_prtn = (osm_prtn_t *)
                cl_qmap_get(&p_qos_policy->p_subn->prtn_pkey_tbl, pkey);

            if (p_prtn == (osm_prtn_t *)cl_qmap_end(
                  &p_qos_policy->p_subn->prtn_pkey_tbl))
                p_prtn = NULL;

            if (is_ipoib) {
                /*
                 * Be very strict for IPoIB partition:
                 *  - the partition for the pkey have to exist
                 *  - it has to have at least 2 full members
                 */
                if (!p_prtn) {
                    yyerror("IPoIB partition, pkey 0x%04X - "
                                       "partition doesn't exist",
                                       cl_ntoh16(pkey));
                    return 1;
                }
                else if (cl_map_count(&p_prtn->full_guid_tbl) < 2) {
                    yyerror("IPoIB partition, pkey 0x%04X - "
                                       "partition has less than two full members",
                                       cl_ntoh16(pkey));
                    return 1;
                }
            }
            else if (!p_prtn) {
                /*
                 * For non-IPoIB pkey we just want to check that
                 * the relevant partition exists.
                 * And even if it doesn't, don't exit - just print
                 * error message and continue.
                 */
                 OSM_LOG(p_qos_parser_osm_log, OSM_LOG_ERROR, "ERR AC02: "
			 "pkey 0x%04X - partition doesn't exist",
                         cl_ntoh16(pkey));
            }
        }
    }