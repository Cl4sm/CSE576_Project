osm_qos_level_t * osm_qos_policy_get_qos_level_by_mpr(
	IN const osm_qos_policy_t * p_qos_policy,
	IN const ib_multipath_rec_t * p_mpr,
	IN const osm_physp_t * p_src_physp,
	IN const osm_physp_t * p_dest_physp,
	IN ib_net64_t comp_mask)
{
	ib_net64_t pr_comp_mask = 0;

	if (!p_qos_policy)
		return NULL;

	/*
	 * Converting MultiPathRecord compmask to the PathRecord
	 * compmask. Note that only relevant bits are set.
	 */
	pr_comp_mask =
		((comp_mask & IB_MPR_COMPMASK_QOS_CLASS) ?
		 IB_PR_COMPMASK_QOS_CLASS : 0) |
		((comp_mask & IB_MPR_COMPMASK_PKEY) ?
		 IB_PR_COMPMASK_PKEY : 0) |
		((comp_mask & IB_MPR_COMPMASK_SERVICEID_MSB) ?
		 IB_PR_COMPMASK_SERVICEID_MSB : 0) |
		((comp_mask & IB_MPR_COMPMASK_SERVICEID_LSB) ?
		 IB_PR_COMPMASK_SERVICEID_LSB : 0);

	return __qos_policy_get_qos_level_by_params(
		p_qos_policy, p_src_physp, p_dest_physp,
		cl_ntoh64(ib_multipath_rec_service_id(p_mpr)),
		ib_multipath_rec_qos_class(p_mpr),
		cl_ntoh16(p_mpr->pkey), pr_comp_mask);
}