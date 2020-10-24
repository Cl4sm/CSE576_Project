static void trace_init(void)
{
	if (!formats_list) {
		duck_constructor();
		erf_constructor();
		tsh_constructor();
		legacy_constructor();
		atmhdr_constructor();
		linuxnative_constructor();
#ifdef HAVE_LIBPCAP
		pcap_constructor();
#endif
		bpf_constructor();
		pcapfile_constructor();
		rt_constructor();
#ifdef HAVE_DAG
		dag_constructor();
#endif
#ifdef HAVE_DPDK
        dpdk_constructor();
#endif
	}
}