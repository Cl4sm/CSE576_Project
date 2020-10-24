static int erf_dump_packet(libtrace_out_t *libtrace,
		dag_record_t *erfptr, unsigned int pad, void *buffer) {
	int numbytes = 0;
	int size;

	if ((numbytes = 
		wandio_wwrite(OUTPUT->file, 
				erfptr,
				(size_t)(dag_record_size + pad))) 
			!= (int)(dag_record_size+pad)) {
		trace_set_err_out(libtrace,errno,
				"write(%s)",libtrace->uridata);
		return -1;
	}

	size=ntohs(erfptr->rlen)-(dag_record_size+pad);
	numbytes=wandio_wwrite(OUTPUT->file, buffer, (size_t)size);
	if (numbytes != size) {
		trace_set_err_out(libtrace,errno,
				"write(%s)",libtrace->uridata);
		return -1;
	}
	return numbytes + pad + dag_record_size;
}