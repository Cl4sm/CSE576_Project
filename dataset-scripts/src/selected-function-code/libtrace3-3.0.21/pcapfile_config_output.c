static int pcapfile_config_output(libtrace_out_t *libtrace,
		trace_option_output_t option,
		void *value)
{
	switch (option) {
		case TRACE_OPTION_OUTPUT_COMPRESS:
			DATAOUT(libtrace)->level = *(int*)value;
			return 0;
		case TRACE_OPTION_OUTPUT_COMPRESSTYPE:
			DATAOUT(libtrace)->compress_type = *(int*)value;
			return 0;
		case TRACE_OPTION_OUTPUT_FILEFLAGS:
			DATAOUT(libtrace)->flag = *(int*)value;
			return 0;
		default:
			/* Unknown option */
			trace_set_err_out(libtrace,TRACE_ERR_UNKNOWN_OPTION,
					"Unknown option");
			return -1;
	}
	return -1;
}