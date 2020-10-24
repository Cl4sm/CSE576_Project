iow_t *trace_open_file_out(libtrace_out_t *trace, int compress_type, int level, int fileflag)
{
	iow_t *io = NULL;

        if (level < 0 || level > 9) {
                trace_set_err_out(trace, TRACE_ERR_UNSUPPORTED_COMPRESS, 
                                "Compression level %d is invalid, must be between 0 and 9 inclusive", 
                                level);
                return NULL;
        }

        if (compress_type < 0 || 
                        compress_type >= TRACE_OPTION_COMPRESSTYPE_LAST) {
                trace_set_err_out(trace, TRACE_ERR_UNSUPPORTED_COMPRESS,
                                "Invalid compression type %d", compress_type);
                return NULL;
        }

	io = wandio_wcreate(trace->uridata, compress_type, level, fileflag);

	if (!io) {
		trace_set_err_out(trace, errno, "Unable to create output file %s", trace->uridata);
	}
	return io;
}