DLLEXPORT libtrace_filter_t *
trace_create_filter_from_bytecode(void *bf_insns, unsigned int bf_len)
{
#ifndef HAVE_BPF_FILTER
	fprintf(stderr, "This version of libtrace does not have BPF support\n");
	return NULL;
#else
	struct libtrace_filter_t *filter = (struct libtrace_filter_t *)
		malloc(sizeof(struct libtrace_filter_t));
	filter->filter.bf_insns = (struct bpf_insn *)
		malloc(sizeof(struct bpf_insn) * bf_len);
	
	memcpy(filter->filter.bf_insns, bf_insns,
			bf_len * sizeof(struct bpf_insn));
	
	filter->filter.bf_len = bf_len;
	filter->filterstring = NULL;
	filter->jitfilter = NULL;
	/* "flag" indicates that the filter member is valid */
	filter->flag = 1; 
	
	return filter;
#endif
}