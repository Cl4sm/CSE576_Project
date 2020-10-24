void autofs_sasl_dispose(struct lookup_context *ctxt)
{
	int status, ret;

	if (ctxt->sasl_conn) {
		sasl_dispose(&ctxt->sasl_conn);
		ctxt->sasl_conn = NULL;
	}

	if (ctxt->kinit_successful) {
		status = pthread_mutex_lock(&krb5cc_mutex);
		if (status)
			fatal(status);

		if (--krb5cc_in_use || ctxt->client_cc)
			ret = krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
		else 
			ret = krb5_cc_destroy(ctxt->krb5ctxt, ctxt->krb5_ccache);
		if (ret)
			logmsg("krb5_cc_destroy failed with non-fatal error %d",
			     ret);

		status = pthread_mutex_unlock(&krb5cc_mutex);
		if (status)
			fatal(status);

		krb5_free_context(ctxt->krb5ctxt);
		if (unsetenv(krb5ccenv) != 0)
			logerr("unsetenv failed with error %d", errno);

		ctxt->krb5ctxt = NULL;
		ctxt->krb5_ccache = NULL;
		ctxt->kinit_done = 0;
		ctxt->kinit_successful = 0;
	}
}