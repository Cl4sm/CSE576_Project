DLLEXPORT int trace_seek_erf_timestamp(libtrace_t *trace, uint64_t ts)
{
	if (trace->format->seek_erf) {
		return trace->format->seek_erf(trace,ts);
	}
	else {
		if (trace->format->seek_timeval) {
			struct timeval tv;
#if __BYTE_ORDER == __BIG_ENDIAN
			tv.tv_sec = ts & 0xFFFFFFFF;
			tv.tv_usec = ((ts >> 32) * 1000000) & 0xFFFFFFFF;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
			tv.tv_sec = ts >> 32;
			tv.tv_usec = ((ts&0xFFFFFFFF)*1000000)>>32;
#else
#error "What on earth are you running this on?"
#endif
			if (tv.tv_usec >= 1000000) {
				tv.tv_usec -= 1000000;
				tv.tv_sec += 1;
			}
			return trace->format->seek_timeval(trace,tv);
		}
		if (trace->format->seek_seconds) {
			double seconds =  
				(ts>>32) + ((ts & UINT_MAX)*1.0 / UINT_MAX);
			return trace->format->seek_seconds(trace,seconds);
		}
		trace_set_err(trace,
				TRACE_ERR_OPTION_UNAVAIL,
				"Feature unimplemented");
		return -1;
	}
}