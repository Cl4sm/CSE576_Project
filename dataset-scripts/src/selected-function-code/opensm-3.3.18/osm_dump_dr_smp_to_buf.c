static void osm_dump_dr_smp_to_buf(IN const ib_smp_t * p_smp, OUT char * buf,
				   IN size_t buf_size)
{
	if (!buf || !p_smp)
		return;
	else {
		unsigned n;

		n = sprintf(buf,
			    "SMP dump:\n"
			    "\t\t\t\tbase_ver................0x%X\n"
			    "\t\t\t\tmgmt_class..............0x%X\n"
			    "\t\t\t\tclass_ver...............0x%X\n"
			    "\t\t\t\tmethod..................0x%X (%s)\n",
			    p_smp->base_ver, p_smp->mgmt_class,
			    p_smp->class_ver, p_smp->method,
			    ib_get_sm_method_str(p_smp->method));

		if (p_smp->mgmt_class == IB_MCLASS_SUBN_DIR) {
			n += snprintf(buf + n, buf_size - n,
				      "\t\t\t\tD bit...................0x%X\n"
				      "\t\t\t\tstatus..................0x%X\n",
				      ib_smp_is_d(p_smp),
				      cl_ntoh16(ib_smp_get_status(p_smp)));
		} else {
			n += snprintf(buf + n, buf_size - n,
				      "\t\t\t\tstatus..................0x%X\n",
				      cl_ntoh16(p_smp->status));
		}

		n += snprintf(buf + n, buf_size - n,
			      "\t\t\t\thop_ptr.................0x%X\n"
			      "\t\t\t\thop_count...............0x%X\n"
			      "\t\t\t\ttrans_id................0x%" PRIx64 "\n"
			      "\t\t\t\tattr_id.................0x%X (%s)\n"
			      "\t\t\t\tresv....................0x%X\n"
			      "\t\t\t\tattr_mod................0x%X\n"
			      "\t\t\t\tm_key...................0x%016" PRIx64
			      "\n", p_smp->hop_ptr, p_smp->hop_count,
			      cl_ntoh64(p_smp->trans_id),
			      cl_ntoh16(p_smp->attr_id),
			      ib_get_sm_attr_str(p_smp->attr_id),
			      cl_ntoh16(p_smp->resv),
			      cl_ntoh32(p_smp->attr_mod),
			      cl_ntoh64(p_smp->m_key));

		if (p_smp->mgmt_class == IB_MCLASS_SUBN_DIR) {
			uint32_t i;
			n += snprintf(buf + n, buf_size - n,
				      "\t\t\t\tdr_slid.................%u\n"
				      "\t\t\t\tdr_dlid.................%u\n",
				      cl_ntoh16(p_smp->dr_slid),
				      cl_ntoh16(p_smp->dr_dlid));

			n += snprintf(buf + n, buf_size - n,
				      "\n\t\t\t\tInitial path: ");
			n += sprint_uint8_arr(buf + n, buf_size - n,
					      p_smp->initial_path,
					      p_smp->hop_count + 1);

			n += snprintf(buf + n, buf_size - n,
				      "\n\t\t\t\tReturn path:  ");
			n += sprint_uint8_arr(buf + n, buf_size - n,
					      p_smp->return_path,
					      p_smp->hop_count + 1);

			n += snprintf(buf + n, buf_size - n,
				      "\n\t\t\t\tReserved:     ");
			for (i = 0; i < 7; i++) {
				n += snprintf(buf + n, buf_size - n,
					      "[%0X]", p_smp->resv1[i]);
			}
			n += snprintf(buf + n, buf_size - n, "\n");

			for (i = 0; i < 64; i += 16) {
				n += snprintf(buf + n, buf_size - n,
					      "\n\t\t\t\t%02X %02X %02X %02X "
					      "%02X %02X %02X %02X"
					      "   %02X %02X %02X %02X %02X %02X %02X %02X\n",
					      p_smp->data[i],
					      p_smp->data[i + 1],
					      p_smp->data[i + 2],
					      p_smp->data[i + 3],
					      p_smp->data[i + 4],
					      p_smp->data[i + 5],
					      p_smp->data[i + 6],
					      p_smp->data[i + 7],
					      p_smp->data[i + 8],
					      p_smp->data[i + 9],
					      p_smp->data[i + 10],
					      p_smp->data[i + 11],
					      p_smp->data[i + 12],
					      p_smp->data[i + 13],
					      p_smp->data[i + 14],
					      p_smp->data[i + 15]);
			}
		} else {
			/* not a Direct Route so provide source and destination lids */
			n += snprintf(buf + n, buf_size - n,
				      "\t\t\t\tMAD IS LID ROUTED\n");
		}
	}
}