static struct osm_remote_node *
switch_find_guid_common(IN const osm_switch_t * p_sw,
			IN struct osm_remote_guids_count *r,
			IN uint8_t port_num, IN int find_sys_guid,
			IN int find_node_guid)
{
	struct osm_remote_node *p_remote_guid = NULL;
	osm_physp_t *p_physp;
	osm_physp_t *p_rem_physp;
	osm_node_t *p_rem_node;
	uint64_t sys_guid;
	uint64_t node_guid;
	unsigned int i;

	CL_ASSERT(p_sw);

	if (!r)
		goto out;

	p_physp = osm_node_get_physp_ptr(p_sw->p_node, port_num);
	p_rem_physp = osm_physp_get_remote(p_physp);
	p_rem_node = osm_physp_get_node_ptr(p_rem_physp);
	sys_guid = p_rem_node->node_info.sys_guid;
	node_guid = p_rem_node->node_info.node_guid;

	for (i = 0; i < r->count; i++) {
		if ((!find_sys_guid
		     || r->guids[i].node->node_info.sys_guid == sys_guid)
		    && (!find_node_guid
			|| r->guids[i].node->node_info.node_guid == node_guid)) {
			p_remote_guid = &r->guids[i];
			break;
		}
	}

out:
	return p_remote_guid;
}