DLLEXPORT int trace_seek_timeval(libtrace_t *trace, struct timeval tv)
{
	if (trace->format->seek_timeval) {
		return trace->format->seek_timeval(trace,tv);
	}
	else {
		if (trace->format->seek_erf) {
			uint64_t timestamp = ((((uint64_t)tv.tv_sec) << 32) + \
				(((uint64_t)tv.tv_usec * UINT_MAX)/1000000));
			return trace->format->seek_erf(trace,timestamp);
		}
		if (trace->format->seek_seconds) {
			double seconds = tv.tv_sec + ((tv.tv_usec * 1.0)/1000000);
			return trace->format->seek_seconds(trace,seconds);
		}
		trace_set_err(trace,
				TRACE_ERR_OPTION_UNAVAIL,
				"Feature unimplemented");
		return -1;
	}
}