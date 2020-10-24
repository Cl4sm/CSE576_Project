DLLEXPORT int trace_seek_seconds(libtrace_t *trace, double seconds)
{
	if (trace->format->seek_seconds) {
		return trace->format->seek_seconds(trace,seconds);
	}
	else {
		if (trace->format->seek_timeval) {
			struct timeval tv;
			tv.tv_sec = (uint32_t)seconds;
			tv.tv_usec = (uint32_t)(((seconds - tv.tv_sec) * 1000000)/UINT_MAX);
			return trace->format->seek_timeval(trace,tv);
		}
		if (trace->format->seek_erf) {
			uint64_t timestamp = 
				((uint64_t)((uint32_t)seconds) << 32) + \
			    (uint64_t)(( seconds - (uint32_t)seconds   ) * UINT_MAX);
			return trace->format->seek_erf(trace,timestamp);
		}
		trace_set_err(trace,
				TRACE_ERR_OPTION_UNAVAIL,
				"Feature unimplemented");
		return -1;
	}
}