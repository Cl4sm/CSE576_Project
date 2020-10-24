void __osm_transport_gen_dummy_mad(osmv_bind_obj_t * p_bo)
{
	struct ib_mad ts_mad;
	osmv_TOPSPIN_transport_mgr_t *p_mgr =
	    (osmv_TOPSPIN_transport_mgr_t *) (p_bo->p_transp_mgr);
	struct ib_get_port_info_ioctl port_data;
	int ts_ioctl_ret;

	/* prepare the mad fields following the stored filter on the bind */
	memset(&ts_mad, 0, sizeof(ts_mad));
	ts_mad.format_version = 1;
	ts_mad.mgmt_class = p_mgr->filter.mgmt_class;
	ts_mad.attribute_id = 0x2;
	ts_mad.class_version = 1;
	ts_mad.r_method = cl_ntoh16(0x2);
	ts_mad.port = p_bo->port_num;
	ts_mad.sqpn = p_mgr->filter.qpn;
	ts_mad.dqpn = p_mgr->filter.qpn;
	ts_mad.slid = 0xffff;
	/* we must send to our local lid ... */
	port_data.port = p_bo->port_num;
	ts_ioctl_ret = ioctl(p_mgr->device_fd, TS_IB_IOCGPORTINFO, &port_data);
	ts_mad.dlid = port_data.port_info.lid;
	ts_mad.transaction_id = 0x9999;
	write(p_mgr->device_fd, &ts_mad, sizeof(ts_mad));
}