DLLEXPORT void trace_perror(libtrace_t *trace,const char *msg,...)
{
	char buf[256];
	va_list va;
	va_start(va,msg);
	vsnprintf(buf,sizeof(buf),msg,va);
	va_end(va);
	if(trace->err.err_num) {
		if (trace->uridata) {
			fprintf(stderr,"%s(%s): %s\n",
					buf,trace->uridata,trace->err.problem);
		} else {
			fprintf(stderr,"%s: %s\n", buf, trace->err.problem);
		}
	} else {
		if (trace->uridata) {
			fprintf(stderr,"%s(%s): No error\n",buf,trace->uridata);
		} else {
			fprintf(stderr,"%s: No error\n", buf);
		}
	}
	trace->err.err_num = 0; /* "OK" */
	trace->err.problem[0]='\0';
}