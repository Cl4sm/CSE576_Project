ib_api_status_t osmtest_run(IN osmtest_t * const p_osmt)
{
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);
	if (p_osmt->opt.flow != OSMT_FLOW_CREATE_INVENTORY &&
	    p_osmt->opt.with_grh) {
		/* Get SM GID for GRH DGID */
		status = osmtest_get_sm_gid(p_osmt);
		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"ERR 0153: calculating SM GID failed: (%s)\n",
				ib_get_err_str(status));
			goto Exit;
		}
	}

	status = osmtest_validate_sa_class_port_info(p_osmt);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0138: "
			"Could not obtain SA ClassPortInfo (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

	if (p_osmt->opt.flow == OSMT_FLOW_CREATE_INVENTORY) {
		/*
		 * Creating an inventory file with all nodes, ports and paths
		 */
		status = osmtest_create_inventory_file(p_osmt);
		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0139: "
				"Inventory file create failed (%s)\n",
				ib_get_err_str(status));
			goto Exit;
		}
	} else {
		if (p_osmt->opt.flow == OSMT_FLOW_STRESS_SA) {
			/*
			 * Stress SA - flood the SA with queries
			 */
			switch (p_osmt->opt.stress) {
			case 0:
			case 1:	/* small response SA query stress */
				status = osmtest_stress_small_rmpp(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0140: "
						"Small RMPP stress test failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
				break;
			case 2:	/* large response SA query stress */
				status = osmtest_stress_large_rmpp(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0141: "
						"Large RMPP stress test failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
				break;
			case 3:	/* large response Path Record SA query stress */
				status = osmtest_create_db(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0142: "
						"Database creation failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}

				status = osmtest_stress_large_rmpp_pr(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0143: "
						"Large RMPP stress test failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
				break;
			case 4: /* SA Get PR to SA LID */
				status = osmtest_stress_get_pr(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 014B: "
						"SA Get PR stress test failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
				break;
			default:
				OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
					"ERR 0144: "
					"Unknown stress test value %u\n",
					p_osmt->opt.stress);
				break;
			}
		} else {

			/*
			 * Run normal validation tests.
			 */
			if (p_osmt->opt.flow == OSMT_FLOW_ALL ||
			    p_osmt->opt.flow == OSMT_FLOW_VALIDATE_INVENTORY) {
				/*
				 * Only validate the given inventory file
				 */
				status = osmtest_create_db(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0145: "
						"Database creation failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}

				status = osmtest_validate_against_db(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0146: "
						"SA validation database failure (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
			}

			if (p_osmt->opt.flow == OSMT_FLOW_ALL) {
				status = osmtest_wrong_sm_key_ignored(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0147: "
						"Try wrong SM_Key failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
			}

			if (p_osmt->opt.flow == OSMT_FLOW_ALL ||
			    p_osmt->opt.flow == OSMT_FLOW_SERVICE_REGISTRATION)
			{
				/*
				 * run service registration, deregistration, and lease test
				 */
				status = osmt_run_service_records_flow(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0148: "
						"Service Flow failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
			}

			if (p_osmt->opt.flow == OSMT_FLOW_ALL ||
			    p_osmt->opt.flow == OSMT_FLOW_EVENT_FORWARDING) {
				/*
				 * Run event forwarding test
				 */
#ifdef OSM_VENDOR_INTF_MTL
				status = osmt_run_inform_info_flow(p_osmt);

				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0149: "
						"Inform Info Flow failed: (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
#else
				OSM_LOG(&p_osmt->log, OSM_LOG_INFO,
					"The event forwarding flow "
					"is not implemented yet!\n");
				status = IB_SUCCESS;
				goto Exit;
#endif
			}

			if (p_osmt->opt.flow == OSMT_FLOW_QOS) {
				/*
				 * QoS info: dump VLArb and SLtoVL tables.
				 * Since it generates a huge file, we run it only
				 * if explicitly required to
				 */
				status = osmtest_create_db(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 014A: "
						"Database creation failed (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}

				status =
				    osmt_run_slvl_and_vlarb_records_flow
				    (p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0150: "
						"Failed to get SLtoVL and VL Arbitration Tables (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
			}

			if (p_osmt->opt.flow == OSMT_FLOW_TRAP) {
				/*
				 * Run trap 64/65 flow (this flow requires running of external tool)
				 */
#ifdef OSM_VENDOR_INTF_MTL
				status = osmt_run_trap64_65_flow(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0151: "
						"Trap 64/65 Flow failed: (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
#else
				OSM_LOG(&p_osmt->log, OSM_LOG_INFO,
					"Trap 64/65 flow "
					"is not implemented yet!\n");
				status = IB_SUCCESS;
				goto Exit;
#endif
			}

			if (p_osmt->opt.flow == OSMT_FLOW_ALL ||
			    p_osmt->opt.flow == OSMT_FLOW_MULTICAST) {
				/*
				 * Multicast flow
				 */
				status = osmt_run_mcast_flow(p_osmt);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0152: "
						"Multicast Flow failed: (%s)\n",
						ib_get_err_str(status));
					goto Exit;
				}
			}

			OSM_LOG(&p_osmt->log, OSM_LOG_INFO,
				"\n\n***************** ALL TESTS PASS *****************\n\n");

		}
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}