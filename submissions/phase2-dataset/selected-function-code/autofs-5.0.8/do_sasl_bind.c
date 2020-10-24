int
do_sasl_bind(unsigned logopt, LDAP *ld, sasl_conn_t *conn, const char **clientout,
	     unsigned int *clientoutlen, const char *auth_mech, int sasl_result)
{
	int ret, msgid, bind_result = LDAP_OTHER;
	struct berval client_cred, *server_cred, temp_cred;
	LDAPMessage *results;
	int have_data, expected_data;

	do {
		/* Take whatever client data we have and send it to the
		 * server. */
		client_cred.bv_val = (char *)*clientout;
		client_cred.bv_len = *clientoutlen;
		ret = ldap_sasl_bind(ld, NULL, auth_mech,
				     (client_cred.bv_len > 0) ?
				     &client_cred : NULL,
				     NULL, NULL, &msgid);
		if (ret != LDAP_SUCCESS) {
			crit(logopt,
			     "Error sending sasl_bind request to "
			     "the server: %s", ldap_err2string(ret));
			return -1;
		}

		/* Wait for a result message for this bind request. */
		results = NULL;
		ret = ldap_result(ld, msgid, LDAP_MSG_ALL, NULL, &results);
		if (ret != LDAP_RES_BIND) {
			crit(logopt,
			     "Error while waiting for response to "
			     "sasl_bind request: %s", ldap_err2string(ret));
			return -1;
		}

		/* Retrieve the result code for the bind request and
		 * any data which the server sent. */
		server_cred = NULL;
		ret = ldap_parse_sasl_bind_result(ld, results,
						  &server_cred, 0);
		ldap_msgfree(results);

		/* Okay, here's where things get tricky.  Both
		 * Mozilla's LDAP SDK and OpenLDAP store the result
		 * code which was returned by the server in the
		 * handle's ERROR_NUMBER option.  Mozilla returns
		 * LDAP_SUCCESS if the data was parsed correctly, even
		 * if the result was an error, while OpenLDAP returns
		 * the result code.  I'm leaning toward Mozilla being
		 * more correct.
		 * In either case, we stuff the result into bind_result.
		 */
		if (ret == LDAP_SUCCESS) {
			/* Mozilla? */
			ret = ldap_get_option(ld, LDAP_OPT_RESULT_CODE,
					      &bind_result);
			if (ret != LDAP_SUCCESS) {
				crit(logopt,
				     "Error retrieving response to sasl_bind "
				     "request: %s", ldap_err2string(ret));
				ret = -1;
				break;
			}
		} else {
			/* OpenLDAP? */
			switch (ret) {
			case LDAP_SASL_BIND_IN_PROGRESS:
				bind_result = ret;
				break;
			default:
				warn(logopt,
				     "Error parsing response to sasl_bind "
				     "request: %s.", ldap_err2string(ret));
				break;
			}
		}

		/*
		 * The LDAP server is supposed to send a NULL value for
		 * server_cred if there was no data.  However, *some*
		 * server implementations get this wrong, and instead send
		 * an empty string.  We check for both.
		 */
		have_data = server_cred != NULL && server_cred->bv_len > 0;

		/*
		 * If the result of the sasl_client_start is SASL_CONTINUE,
		 * then the server should have sent us more data.
		 */
		expected_data = sasl_result == SASL_CONTINUE;

		if (have_data && !expected_data) {
			warn(logopt,
			     "The LDAP server sent data in response to our "
			     "bind request, but indicated that the bind was "
			     "complete. LDAP SASL bind with mechansim %s "
			     "failed.", auth_mech);
			ret = -1;
			break;
		}
		if (expected_data && !have_data) {
			warn(logopt,
			     "The LDAP server indicated that the LDAP SASL "
			     "bind was incomplete, but did not provide the "
			     "required data to proceed. LDAP SASL bind with "
			     "mechanism %s failed.", auth_mech);
			ret = -1;
			break;
		}

		/* If we need another round trip, process whatever we
		 * received and prepare data to be transmitted back. */
		if ((sasl_result == SASL_CONTINUE) &&
		    ((bind_result == LDAP_SUCCESS) ||
		     (bind_result == LDAP_SASL_BIND_IN_PROGRESS))) {
			if (server_cred != NULL) {
				temp_cred = *server_cred;
			} else {
				temp_cred.bv_len = 0;
				temp_cred.bv_val = NULL;
			}
			sasl_result = sasl_client_step(conn,
						       temp_cred.bv_val,
						       temp_cred.bv_len,
						       NULL,
						       clientout,
						       clientoutlen);
			/* If we have data to send, then the server
			 * had better be expecting it.  (It's valid
			 * to send the server no data with a request.)
			 */
			if ((*clientoutlen > 0) &&
			    (bind_result != LDAP_SASL_BIND_IN_PROGRESS)) {
				warn(logopt,
				     "We have data for the server, "
				     "but it thinks we are done!");
				/* XXX should print out debug data here */
				ret = -1;
			}
		}

		if (server_cred && server_cred->bv_len > 0) {
			ber_bvfree(server_cred);
			server_cred = NULL;
		}

	} while ((bind_result == LDAP_SASL_BIND_IN_PROGRESS) ||
		 (sasl_result == SASL_CONTINUE));

	if (server_cred && server_cred->bv_len > 0)
		ber_bvfree(server_cred);

	return ret;
}