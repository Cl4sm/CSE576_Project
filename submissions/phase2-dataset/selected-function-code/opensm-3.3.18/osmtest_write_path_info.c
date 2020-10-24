static ib_api_status_t
osmtest_write_path_info(IN osmtest_t * const p_osmt,
			IN FILE * fh, IN const ib_path_rec_t * const p_rec)
{
	int result;
	cl_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	result = fprintf(fh,
			 "DEFINE_PATH\n"
			 "dgid                    0x%016" PRIx64 " 0x%016"
			 PRIx64 "\nsgid                    0x%016" PRIx64
			 " 0x%016" PRIx64 "\ndlid                    0x%X\n"
			 "slid                    0x%X\n"
			 "# hop_flow_raw          0x%X\n"
			 "# tclass                0x%X\n"
			 "# num_path              0x%X\n"
			 "pkey                    0x%X\n"
			 "# sl                    0x%X\n"
			 "# qos_class             0x%X\n"
			 "# mtu                   0x%X\n"
			 "# rate                  0x%X\n"
			 "# pkt_life              0x%X\n"
			 "# preference            0x%X\n" "END\n\n",
			 cl_ntoh64(p_rec->dgid.unicast.prefix),
			 cl_ntoh64(p_rec->dgid.unicast.interface_id),
			 cl_ntoh64(p_rec->sgid.unicast.prefix),
			 cl_ntoh64(p_rec->sgid.unicast.interface_id),
			 cl_ntoh16(p_rec->dlid), cl_ntoh16(p_rec->slid),
			 cl_ntoh32(p_rec->hop_flow_raw), p_rec->tclass,
			 p_rec->num_path, cl_ntoh16(p_rec->pkey),
			 ib_path_rec_sl(p_rec), ib_path_rec_qos_class(p_rec),
			 p_rec->mtu, p_rec->rate, p_rec->pkt_life,
			 p_rec->preference);

	if (result < 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0162: "
			"Write failed\n");
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}