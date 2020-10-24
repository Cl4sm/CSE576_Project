static void trap_rcv_process_request(IN osm_sm_t * sm,
				     IN const osm_madw_t * p_madw)
{
	uint8_t payload[sizeof(ib_mad_notice_attr_t)];
	ib_smp_t *p_smp;
	ib_mad_notice_attr_t *p_ntci = (ib_mad_notice_attr_t *) payload;
	ib_api_status_t status;
	osm_madw_t tmp_madw;	/* we need a copy to last after repress */
	uint64_t trap_key;
	uint32_t num_received;
	osm_physp_t *p_physp;
	osm_port_t *p_port;
	ib_net16_t source_lid = 0;
	boolean_t is_gsi = TRUE;
	uint8_t port_num = 0;
	boolean_t physp_change_trap = FALSE;
	uint64_t event_wheel_timeout = OSM_DEFAULT_TRAP_SUPRESSION_TIMEOUT;
	boolean_t run_heavy_sweep = FALSE;
	char buf[1024];
	osm_dr_path_t *p_path;
	unsigned n;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	if (osm_exit_flag)
		/*
		   We got an exit flag - do nothing
		   Otherwise we start a sweep on the trap 144 caused by
		   cleaning up SM Cap bit...
		 */
		goto Exit2;

	/* update the is_gsi flag according to the mgmt_class field */
	if (p_madw->p_mad->mgmt_class == IB_MCLASS_SUBN_LID ||
	    p_madw->p_mad->mgmt_class == IB_MCLASS_SUBN_DIR)
		is_gsi = FALSE;

	/* No real need to grab the lock for this function. */
	memset(payload, 0, sizeof(payload));
	memset(&tmp_madw, 0, sizeof(tmp_madw));

	p_smp = osm_madw_get_smp_ptr(p_madw);

	if (p_smp->method != IB_MAD_METHOD_TRAP) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3801: "
			"Unsupported method 0x%X\n", p_smp->method);
		goto Exit2;
	}

	/*
	 * The NOTICE Attribute is part of the SMP CLASS attributes
	 * As such the actual attribute data resides inside the SMP
	 * payload.
	 */

	memcpy(payload, &p_smp->data, IB_SMP_DATA_SIZE);
	memcpy(&tmp_madw, p_madw, sizeof(tmp_madw));

	if (is_gsi == FALSE) {
		/* We are in smi flow */
		/*
		 * When we receive a TRAP with dlid = 0 - it means it
		 * came from our own node. So we need to fix it.
		 */

		if (p_madw->mad_addr.addr_type.smi.source_lid == 0) {
			/* Check if the sm_base_lid is 0. If yes - this means
			   that the local lid wasn't configured yet. Don't send
			   a response to the trap. */
			if (sm->p_subn->sm_base_lid == 0) {
				OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
					"Received SLID=0 Trap with local LID=0. Ignoring MAD\n");
				goto Exit2;
			}
			OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
				"Received SLID=0 Trap. Using local LID:%u instead\n",
				cl_ntoh16(sm->p_subn->sm_base_lid));
			tmp_madw.mad_addr.addr_type.smi.source_lid =
			    sm->p_subn->sm_base_lid;
		}

		source_lid = tmp_madw.mad_addr.addr_type.smi.source_lid;

		/* Print some info about the incoming Trap */
		log_trap_info(sm->p_log, p_ntci, source_lid, p_smp->trans_id);
	}

	osm_dump_notice_v2(sm->p_log, p_ntci, FILE_ID, OSM_LOG_VERBOSE);
	CL_PLOCK_ACQUIRE(sm->p_lock);
	p_physp = osm_get_physp_by_mad_addr(sm->p_log, sm->p_subn,
					    &tmp_madw.mad_addr);
	if (p_physp)
		p_smp->m_key = ib_port_info_get_m_key(&p_physp->port_info);
	else
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3809: "
			"Failed to find source physical port for trap\n");

	status = osm_resp_send(sm, &tmp_madw, 0, payload);
	if (status != IB_SUCCESS) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3802: "
			"Error sending response (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

	/*
	 * We would like to filter out recurring Traps so we track them by
	 * their source lid and content. If the same trap was already
	 * received within the aging time window more than 10 times,
	 * we simply ignore it. This is done only if we are in smi mode
	 */

	if (is_gsi == FALSE) {
		if (ib_notice_is_generic(p_ntci) &&
		    (p_ntci->g_or_v.generic.trap_num == CL_HTON16(SM_LINK_INTEGRITY_THRESHOLD_TRAP) ||
		     p_ntci->g_or_v.generic.trap_num == CL_HTON16(SM_BUFFER_OVERRUN_THRESHOLD_TRAP) ||
		     p_ntci->g_or_v.generic.trap_num == CL_HTON16(SM_WATCHDOG_TIMER_EXPIRED_TRAP))) {
			/* If this is a trap 129, 130, or 131 - then this is a
			 * trap signaling a change on a physical port.
			 * Mark the physp_change_trap flag as TRUE.
			 */
			physp_change_trap = TRUE;
			/* The source_lid should be based on the source_lid from the trap */
			source_lid = p_ntci->data_details.ntc_129_131.lid;
			port_num = p_ntci->data_details.ntc_129_131.port_num;
		}

		/* try to find it in the aging tracker */
		trap_key = trap_get_key(source_lid, port_num, p_ntci);
		num_received = cl_event_wheel_num_regs(&sm->trap_aging_tracker,
						       trap_key);

		/* Now we know how many times it provided this trap */
		if (num_received > 10) {
			if (print_num_received(num_received))
				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Received trap %u times consecutively\n",
					num_received);
			/*
			 * If the trap provides info about a bad port
			 * we mark it as unhealthy.
			 */
			if (physp_change_trap == TRUE) {
				int ret = shutup_noisy_port(sm, source_lid,
							    port_num,
							    num_received);
				if (ret == 1) /* port disabled */
					goto Exit;
				else if (ret == 2) /* unhealthy - run sweep */
					run_heavy_sweep = TRUE;
				/* in any case increase timeout interval */
				event_wheel_timeout =
				    OSM_DEFAULT_UNHEALTHY_TIMEOUT;
			}
		}

		/* restart the aging anyway */
		/* If physp_change_trap is TRUE - then use a callback to unset
		   the healthy bit. If not - no need to use a callback. */
		if (physp_change_trap == TRUE)
			cl_event_wheel_reg(&sm->trap_aging_tracker, trap_key,
					   cl_get_time_stamp() + event_wheel_timeout,
					   aging_tracker_callback, sm);
		else
			cl_event_wheel_reg(&sm->trap_aging_tracker, trap_key,
					   cl_get_time_stamp() + event_wheel_timeout,
					   NULL, NULL);

		/* If was already registered do nothing more */
		if (num_received > 10 && run_heavy_sweep == FALSE) {
			if (print_num_received(num_received))
				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Ignoring noisy traps.\n");
			goto Exit;
		}
	}

	/* Check for node description update. IB Spec v1.2.1 pg 823 */
	if (!ib_notice_is_generic(p_ntci))
		goto check_sweep;
	if (cl_ntoh16(p_ntci->g_or_v.generic.trap_num) == SM_LOCAL_CHANGES_TRAP &&
	    p_ntci->data_details.ntc_144.local_changes & TRAP_144_MASK_OTHER_LOCAL_CHANGES &&
	    p_ntci->data_details.ntc_144.change_flgs & TRAP_144_MASK_NODE_DESCRIPTION_CHANGE) {
		OSM_LOG(sm->p_log, OSM_LOG_INFO, "Trap 144 Node description update\n");

		if (p_physp) {
			osm_req_get_node_desc(sm, p_physp);
			if (!(p_ntci->data_details.ntc_144.change_flgs & ~TRAP_144_MASK_NODE_DESCRIPTION_CHANGE) &&
			    p_ntci->data_details.ntc_144.new_cap_mask == p_physp->port_info.capability_mask)
				goto check_report;
		} else
			OSM_LOG(sm->p_log, OSM_LOG_ERROR,
				"ERR 3812: No physical port found for "
				"trap 144: \"node description update\"\n");
		goto check_sweep;
	} else if (cl_ntoh16(p_ntci->g_or_v.generic.trap_num) == SM_SYS_IMG_GUID_CHANGED_TRAP) {
		if (p_physp) {
			CL_PLOCK_RELEASE(sm->p_lock);
			CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
			p_physp = osm_get_physp_by_mad_addr(sm->p_log,
							    sm->p_subn,
							    &tmp_madw.mad_addr);
			if (p_physp) {
				/* this assumes that trap 145 content is not broken? */
				p_physp->p_node->node_info.sys_guid =
					p_ntci->data_details.ntc_145.new_sys_guid;
			}
			CL_PLOCK_RELEASE(sm->p_lock);
			CL_PLOCK_ACQUIRE(sm->p_lock);
		} else
			OSM_LOG(sm->p_log, OSM_LOG_ERROR,
				"ERR 3813: No physical port found for "
				"trap 145: \"SystemImageGUID update\"\n");
		goto check_report;
	}

check_sweep:
	if (osm_log_is_active_v2(sm->p_log, OSM_LOG_INFO, FILE_ID)) {
		if (ib_notice_is_generic(p_ntci) &&
		    cl_ntoh16(p_ntci->g_or_v.generic.trap_num) == SM_LINK_STATE_CHANGED_TRAP) {
			p_path = (p_physp) ?
			    osm_physp_get_dr_path_ptr(p_physp) : NULL;
			if (p_path) {
				n = sprintf(buf, "SM class trap %u: ",
					    cl_ntoh16(p_ntci->g_or_v.generic.trap_num));
				n += snprintf(buf + n, sizeof(buf) - n,
					      "Directed Path Dump of %u hop path: "
					      "Path = ", p_path->hop_count);

				osm_dump_dr_path_as_buf(sizeof(buf) - n, p_path,
							buf + n);

				osm_log_v2(sm->p_log, OSM_LOG_INFO, FILE_ID,
					   "%s\n", buf);
			}
		}
	}

	/* do a sweep if we received a trap */
	if (sm->p_subn->opt.sweep_on_trap) {
		/* if this is trap number 128 or run_heavy_sweep is TRUE -
		   update the force_heavy_sweep flag of the subnet.
		   Sweep also on traps 144 - these traps signal a change of
		   certain port capabilities.
		   TODO: In the future this can be changed to just getting
		   PortInfo on this port instead of sweeping the entire subnet. */
		if (ib_notice_is_generic(p_ntci) &&
		    (cl_ntoh16(p_ntci->g_or_v.generic.trap_num) == SM_LINK_STATE_CHANGED_TRAP ||
		     cl_ntoh16(p_ntci->g_or_v.generic.trap_num) == SM_LOCAL_CHANGES_TRAP ||
		     run_heavy_sweep)) {
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"Forcing heavy sweep. Received trap:%u\n",
				cl_ntoh16(p_ntci->g_or_v.generic.trap_num));

			sm->p_subn->force_heavy_sweep = TRUE;
		}
		osm_sm_signal(sm, OSM_SIGNAL_SWEEP);
	}

	/* If we reached here due to trap 129/130/131 - do not need to do
	   the notice report. Just goto exit. We know this is the case
	   if physp_change_trap is TRUE. */
	if (physp_change_trap == TRUE)
		goto Exit;

check_report:
	/* We are going to report the notice - so need to fix the IssuerGID
	   accordingly. See IBA 1.2 p.739 or IBA 1.1 p.653 for details. */
	if (is_gsi) {
		if (!tmp_madw.mad_addr.addr_type.gsi.global_route) {
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3806: "
				"Received gsi trap with global_route FALSE. "
				"Cannot update issuer_gid!\n");
			goto Exit;
		}
		memcpy(&p_ntci->issuer_gid,
		       &tmp_madw.mad_addr.addr_type.gsi.grh_info.src_gid,
		       sizeof(ib_gid_t));
	} else {
		/* Need to use the IssuerLID */
		p_port = osm_get_port_by_lid(sm->p_subn, source_lid);
		if (!p_port) {
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"Cannot find port corresponding to lid:%u\n",
				cl_ntoh16(source_lid));

			goto Exit;
		}

		p_ntci->issuer_gid.unicast.prefix =
		    sm->p_subn->opt.subnet_prefix;
		p_ntci->issuer_gid.unicast.interface_id = p_port->guid;
	}

	/* we need a lock here as the InformInfo DB must be stable */
	status = osm_report_notice(sm->p_log, sm->p_subn, p_ntci);
	if (status != IB_SUCCESS) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3803: "
			"Error sending trap reports (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

Exit:
	CL_PLOCK_RELEASE(sm->p_lock);
Exit2:
	OSM_LOG_EXIT(sm->p_log);
}