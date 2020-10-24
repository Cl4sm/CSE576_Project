static int rt_set_format(libtrace_t *libtrace, libtrace_packet_t *packet) 
{

	/* We need to assign the packet to a "dead" trace */

	/* Try to minimize the number of corrupt packets that slip through
	 * while making it easy to identify new pcap DLTs */
	if (packet->type > TRACE_RT_DATA_DLT && 
			packet->type < TRACE_RT_DATA_DLT_END) {
		if (!RT_INFO->dummy_pcap) {
			RT_INFO->dummy_pcap = trace_create_dead("pcap:-");
		}
		packet->trace = RT_INFO->dummy_pcap;
		return 0;	
	}

	if (packet->type > TRACE_RT_DATA_BPF &&
			packet->type < TRACE_RT_DATA_BPF_END) {

		if (!RT_INFO->dummy_bpf) {
			RT_INFO->dummy_bpf = trace_create_dead("bpf:-");
			/* This may fail on a non-BSD machine */
			if (trace_is_err(RT_INFO->dummy_bpf)) {
				trace_perror(RT_INFO->dummy_bpf, "Creating dead bpf trace");
				return -1;
			}
		}
		packet->trace = RT_INFO->dummy_bpf;
		return 0;
	}

	switch (packet->type) {
		case TRACE_RT_DUCK_2_4:
		case TRACE_RT_DUCK_2_5:
			if (!RT_INFO->dummy_duck) {
				RT_INFO->dummy_duck = trace_create_dead("duck:dummy");
			}
			packet->trace = RT_INFO->dummy_duck;
			break;
		case TRACE_RT_DATA_ERF:
			if (!RT_INFO->dummy_erf) {
				RT_INFO->dummy_erf = trace_create_dead("erf:-");
			}
			packet->trace = RT_INFO->dummy_erf;
			break;
		case TRACE_RT_DATA_LINUX_NATIVE:
			if (!RT_INFO->dummy_linux) {
				RT_INFO->dummy_linux = trace_create_dead("int:");
				/* This may fail on a non-Linux machine */
				if (trace_is_err(RT_INFO->dummy_linux)) {
					trace_perror(RT_INFO->dummy_linux, "Creating dead int trace");
					return -1;
				}
			}
			packet->trace = RT_INFO->dummy_linux;
			break;
		case TRACE_RT_DATA_LINUX_RING:
			if (!RT_INFO->dummy_ring) {
				RT_INFO->dummy_ring = trace_create_dead("ring:");
				/* This may fail on a non-Linux machine */
				if (trace_is_err(RT_INFO->dummy_ring)) {
					trace_perror(RT_INFO->dummy_ring, "Creating dead int trace");
					return -1;
				}
			}
			packet->trace = RT_INFO->dummy_ring;
			break;
		case TRACE_RT_STATUS:
		case TRACE_RT_METADATA:
			/* Just use the RT trace! */
			packet->trace = libtrace;
			break;
		case TRACE_RT_DATA_LEGACY_ETH:
		case TRACE_RT_DATA_LEGACY_ATM:
		case TRACE_RT_DATA_LEGACY_POS:
			printf("Sending legacy over RT is currently not supported\n");
			trace_set_err(libtrace, TRACE_ERR_BAD_PACKET, "Legacy packet cannot be sent over rt");
			return -1;
		default:
			printf("Unrecognised format: %u\n", packet->type);
			trace_set_err(libtrace, TRACE_ERR_BAD_PACKET, "Unrecognised packet format");
			return -1;
	}
	return 0; /* success */
}		