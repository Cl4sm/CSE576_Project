static
void torus_update_osm_vlarb(void *context, osm_physp_t *osm_phys_port,
			    uint8_t port_num, ib_vl_arb_table_t *block,
			    unsigned block_length, unsigned block_num)
{
	osm_node_t *node = osm_physp_get_node_ptr(osm_phys_port);
	struct torus_context *ctx = context;
	struct t_switch *sw = NULL;
	unsigned i, next;

	if (node->sw) {
		sw = node->sw->priv;
		if (sw && sw->osm_switch != node->sw) {
			osm_log_t *log = &ctx->osm->log;
			guid_t guid;

			guid = osm_node_get_node_guid(node);
			OSM_LOG(log, OSM_LOG_INFO,
				"Note: osm_switch (GUID 0x%04"PRIx64") "
				"not in torus fabric description\n",
				cl_ntoh64(guid));
			return;
		}
	}

	/*
	 * If osm_phys_port is a switch port that connects to a CA, then
	 * we're using at most VL 0 (for QoS level 0) and VL 1 (for QoS
	 * level 1).  We've been passed the  VLarb values for a switch
	 * external port, so we need to fix them up to avoid unexpected
	 * results depending on how the switch handles VLarb values for
	 * unprogrammed VLs.
	 *
	 * For inter-switch links torus-2QoS uses VLs 0-3 to implement
	 * QoS level 0, and VLs 4-7 to implement QoS level 1.
	 *
	 * So, leave VL 0 alone, remap VL 4 to VL 1, zero out the rest,
	 * and compress out the zero entries to the end.
	 */
	if (!sw || !port_num || !sw->port[port_num] ||
	    sw->port[port_num]->pgrp->port_grp != 2 * TORUS_MAX_DIM)
		return;

	next = 0;
	for (i = 0; i < block_length; i++) {
		switch (block->vl_entry[i].vl) {
		case 4:
			block->vl_entry[i].vl = 1;
			/* fall through */
		case 0:
			block->vl_entry[next].vl = block->vl_entry[i].vl;
			block->vl_entry[next].weight = block->vl_entry[i].weight;
			next++;
			/*
			 * If we didn't update vl_entry[i] in place,
			 * fall through to zero it out.
			 */
			if (next > i)
				break;
		default:
			block->vl_entry[i].vl = 0;
			block->vl_entry[i].weight = 0;
			break;
		}
	}
}