static int remove_path_from_cdg(cdg_node_t ** cdg_root, osm_port_t * src_port,
				uint16_t slid, osm_port_t * dest_port,
				uint16_t dlid)
{
	osm_node_t *local_node = NULL, *remote_node = NULL;
	uint16_t local_lid = 0, remote_lid = 0;
	uint8_t local_port = 0, remote_port = 0;
	uint64_t channelID = 0;

	cdg_node_t *channel_head = NULL, *channel = NULL, *last_channel = NULL;
	cdg_link_t *linklist = NULL;

	channel_head = (cdg_node_t *) malloc(sizeof(cdg_node_t));
	if (!channel_head)
		goto ERROR;
	set_default_cdg_node(channel_head);
	last_channel = channel_head;

	/* if src is a Hca, then the channel from Hca to switch would be a source in the graph
	   sources can't be part of a cycle -> skip this channel
	 */
	remote_node =
	    osm_node_get_remote_node(src_port->p_node,
				     src_port->p_physp->port_num, &remote_port);

	while (remote_node && remote_node->sw) {
		local_node = remote_node;
		local_port = local_node->sw->new_lft[dlid];
		local_lid = cl_ntoh16(osm_node_get_base_lid(local_node, 0));

		remote_node =
		    osm_node_get_remote_node(local_node, local_port,
					     &remote_port);
		/* if remote_node is a Hca, then the last channel from switch to Hca would be a sink in the cdg -> skip */
		if (!remote_node || !remote_node->sw)
			break;
		remote_lid = cl_ntoh16(osm_node_get_base_lid(remote_node, 0));

		channelID =
		    (((uint64_t) local_lid) << 48) +
		    (((uint64_t) local_port) << 32) +
		    (((uint64_t) remote_lid) << 16) + ((uint64_t) remote_port);
		channel = cdg_search(*cdg_root, channelID);
		if (channel) {
			/* check whether last channel has connection to this channel, i.e. subpath already exists in cdg */
			linklist = last_channel->linklist;
			while (linklist && linklist->node != channel
			       && linklist->next)
				linklist = linklist->next;
			/* remove the srcdest from the link */
			if (linklist) {
				if (linklist->node == channel) {
					linklist->removed++;
				} else {
					/* may happen if the link is missing (thru cycle detect algorithm) */
				}
			} else {
				/* may happen if the link is missing (thru cycle detect algorithm or last_channel==channel_head (dummy channel)) */
			}
		} else {
			/* must be an error, channels for the path are added before, so a missing channel would be a corrupt data structure */
			goto ERROR;
		}
		last_channel = channel;
	}

	if (channel_head->linklist)
		free(channel_head->linklist);
	free(channel_head);

	return 0;

ERROR:
	/* cleanup data and exit */
	if (channel_head) {
		if (channel_head->linklist)
			free(channel_head->linklist);
		free(channel_head);
	}

	return 1;
}