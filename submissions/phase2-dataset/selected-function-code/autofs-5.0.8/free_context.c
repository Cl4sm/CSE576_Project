static void free_context(struct lookup_context *ctxt)
{
	int ret;

	if (ctxt->schema) {
		free(ctxt->schema->map_class);
		free(ctxt->schema->map_attr);
		free(ctxt->schema->entry_class);
		free(ctxt->schema->entry_attr);
		free(ctxt->schema->value_attr);
		free(ctxt->schema);
	}
	if (ctxt->auth_conf)
		free(ctxt->auth_conf);
	if (ctxt->sasl_mech)
		free(ctxt->sasl_mech);
	if (ctxt->user)
		free(ctxt->user);
	if (ctxt->secret)
		free(ctxt->secret);
	if (ctxt->client_princ)
		free(ctxt->client_princ);
	if (ctxt->client_cc)
		free(ctxt->client_cc);
	if (ctxt->mapname)
		free(ctxt->mapname);
	if (ctxt->qdn)
		free(ctxt->qdn);
	if (ctxt->server)
		free(ctxt->server);
	if (ctxt->cur_host)
		free(ctxt->cur_host);
	if (ctxt->base)
		free(ctxt->base);
	if (ctxt->uris)
		defaults_free_uris(ctxt->uris);
	ret = pthread_mutex_destroy(&ctxt->uris_mutex);
	if (ret)
		fatal(ret);
	if (ctxt->sdns)
		defaults_free_searchdns(ctxt->sdns);
	if (ctxt->dclist)
		free_dclist(ctxt->dclist);
#ifdef WITH_SASL
	if (ctxt->extern_cert)
		free(ctxt->extern_cert);
	if (ctxt->extern_key)
		free(ctxt->extern_key);
#endif
	free(ctxt);

	return;
}