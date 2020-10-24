static int load_certificate(struct openconnect_info *vpninfo)
{
	gnutls_datum_t fdata;
	gnutls_x509_privkey_t key = NULL;
#if defined(HAVE_P11KIT) || defined(HAVE_TROUSERS)
	gnutls_privkey_t pkey = NULL;
	gnutls_datum_t pkey_sig = {NULL, 0};
	void *dummy_hash_data = &load_certificate;
#endif
#ifdef HAVE_P11KIT
	char *cert_url = (char *)vpninfo->cert;
	char *key_url = (char *)vpninfo->sslkey;
	gnutls_pkcs11_privkey_t p11key = NULL;
#endif
	char *pem_header;
	gnutls_x509_crl_t crl = NULL;
	gnutls_x509_crt_t last_cert, cert = NULL;
	gnutls_x509_crt_t *extra_certs = NULL, *supporting_certs = NULL;
	unsigned int nr_supporting_certs = 0, nr_extra_certs = 0;
	uint8_t *free_supporting_certs = NULL;
	int err; /* GnuTLS error */
	int ret;
	int i;
	int cert_is_p11 = 0, key_is_p11 = 0;
	unsigned char key_id[20];
	size_t key_id_size = sizeof(key_id);
	char name[80];

	fdata.data = NULL;

	key_is_p11 = !strncmp(vpninfo->sslkey, "pkcs11:", 7);
	cert_is_p11 = !strncmp(vpninfo->cert, "pkcs11:", 7);

#ifndef HAVE_P11KIT
	if (key_is_p11 || cert_is_p11) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("This binary built without PKCS#11 support\n"));
		return -EINVAL;
	}
#else
	/* Install PIN handler if either certificate or key are coming from PKCS#11 */
	if (key_is_p11 || cert_is_p11) {
		CK_OBJECT_CLASS class;
		CK_ATTRIBUTE attr;
		char pin_source[40];
		P11KitUri *uri;

		sprintf(pin_source, "openconnect:%p", vpninfo);
		p11_kit_pin_register_callback(pin_source, pin_callback, vpninfo, NULL);

		uri = p11_kit_uri_new();

		attr.type = CKA_CLASS;
		attr.pValue = &class;
		attr.ulValueLen = sizeof(class);

		/* Add appropriate pin-source and object-type attributes to
		   both certificate and key URLs, unless they already exist. */
		if (cert_is_p11 &&
		    !p11_kit_uri_parse(cert_url, P11_KIT_URI_FOR_ANY, uri)) {
			if (!p11_kit_uri_get_pin_source(uri))
				p11_kit_uri_set_pin_source(uri, pin_source);
			if (!p11_kit_uri_get_attribute(uri, CKA_CLASS)) {
				class = CKO_CERTIFICATE;
				p11_kit_uri_set_attribute(uri, &attr);
			}
			p11_kit_uri_format(uri, P11_KIT_URI_FOR_ANY, &cert_url);
		}

		if (key_is_p11 &&
		    !p11_kit_uri_parse(key_url, P11_KIT_URI_FOR_ANY, uri)) {
			if (!p11_kit_uri_get_pin_source(uri))
				p11_kit_uri_set_pin_source(uri, pin_source);
			if (!p11_kit_uri_get_attribute(uri, CKA_CLASS)) {
				class = CKO_PRIVATE_KEY;
				p11_kit_uri_set_attribute(uri, &attr);
			}
			p11_kit_uri_format(uri, P11_KIT_URI_FOR_ANY, &key_url);
		}

		p11_kit_uri_free(uri);
	}

	/* Load certificate(s) first... */
	if (cert_is_p11) {
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("Using PKCS#11 certificate %s\n"), cert_url);

		err = gnutls_x509_crt_init(&cert);
		if (err) {
			ret = -ENOMEM;
			goto out;
		}
		err = gnutls_x509_crt_import_pkcs11_url(cert, cert_url, 0);
		if (err == GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE)
			err = gnutls_x509_crt_import_pkcs11_url(cert, cert_url,
								GNUTLS_PKCS11_OBJ_FLAG_LOGIN);
		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Error loading certificate from PKCS#11: %s\n"),
				     gnutls_strerror(err));
			ret = -EIO;
			goto out;
		}
		goto got_certs;
	}
#endif /* HAVE_P11KIT */

	/* OK, not a PKCS#11 certificate so it must be coming from a file... */
	vpn_progress(vpninfo, PRG_DEBUG,
		     _("Using certificate file %s\n"), vpninfo->cert);

	/* Load file contents */
	ret = load_datum(vpninfo, &fdata, vpninfo->cert);
	if (ret)
		return ret;

	/* Is it PKCS#12? */
	if (!key_is_p11 && (vpninfo->cert_type == CERT_TYPE_PKCS12 ||
			    vpninfo->cert_type == CERT_TYPE_UNKNOWN)) {
		/* PKCS#12 should actually contain certificates *and* private key */
		ret = load_pkcs12_certificate(vpninfo, &fdata, &key,
					      &supporting_certs, &nr_supporting_certs,
					      &extra_certs, &nr_extra_certs,
					      &crl);
		if (ret < 0)
			goto out;
		else if (!ret) {
			if (nr_supporting_certs) {
				cert = supporting_certs[0];
				free_supporting_certs = gnutls_malloc(nr_supporting_certs);
				if (!free_supporting_certs) {
					ret = -ENOMEM;
					goto out;
				}
				memset(free_supporting_certs, 1, nr_supporting_certs);
				goto got_key;
			}
			vpn_progress(vpninfo, PRG_ERR,
				     _("PKCS#11 file contained no certificate\n"));
			ret = -EINVAL;
			goto out;
		}

		/* It returned NOT_PKCS12.
		   Fall through to try PEM formats. */
	}

	/* We need to know how many there are in *advance*; it won't just allocate
	   the array for us :( */
	nr_extra_certs = count_x509_certificates(&fdata);
	if (!nr_extra_certs)
		nr_extra_certs = 1; /* wtf? Oh well, we'll fail later... */

	extra_certs = calloc(nr_extra_certs, sizeof(cert));
	if (!extra_certs) {
		nr_extra_certs = 0;
		ret = -ENOMEM;
		goto out;
	}
	err = gnutls_x509_crt_list_import(extra_certs, &nr_extra_certs, &fdata,
					  GNUTLS_X509_FMT_PEM, 0);
	if (err <= 0) {
		const char *reason;
		if (!err || err == GNUTLS_E_NO_CERTIFICATE_FOUND)
			reason = _("No certificate found in file");
		else
			reason = gnutls_strerror(err);

		vpn_progress(vpninfo, PRG_ERR,
			     _("Loading certificate failed: %s\n"),
			     reason);
		ret = -EINVAL;
		goto out;
	}
	nr_extra_certs = err;
	err = 0;

	goto got_certs;
 got_certs:
	/* Now we have either a single certificate in 'cert', or an array of
	   them in extra_certs[]. Next we look for the private key ... */
#if defined(HAVE_P11KIT)
	if (key_is_p11) {
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("Using PKCS#11 key %s\n"), key_url);

		err = gnutls_pkcs11_privkey_init(&p11key);
		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Error initialising PKCS#11 key structure: %s\n"),
				     gnutls_strerror(err));
			ret = -EIO;
			goto out;
		}

		err = gnutls_pkcs11_privkey_import_url(p11key, key_url, 0);

		/* Annoyingly, some tokens don't even admit the *existence* of
		   the key until they're logged in. And thus a search doesn't
		   work unless it specifies the *token* too. But if the URI for
		   key and cert are the same, and the cert was found, then we
		   can work out what token the *cert* was found in and try that
		   before we give up... */
		if (err == GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE &&
		    vpninfo->cert == vpninfo->sslkey) {
			gnutls_pkcs11_obj_t crt;
			P11KitUri *uri;
			CK_TOKEN_INFO *token;
			char buf[33];
			size_t s;

			if (gnutls_pkcs11_obj_init(&crt))
				goto key_err;
			if (gnutls_pkcs11_obj_import_url(crt, cert_url, 0))
				goto key_err_obj;
			uri = p11_kit_uri_new();
			if (!uri)
				goto key_err_obj;
			if (p11_kit_uri_parse(key_url, P11_KIT_URI_FOR_ANY, uri))
				goto key_err_uri;
			token = p11_kit_uri_get_token_info(uri);
			if (!token)
				goto key_err_uri;

			if (!token->label[0]) {
				s = sizeof(token->label) + 1;
				if (!gnutls_pkcs11_obj_get_info(crt, GNUTLS_PKCS11_OBJ_TOKEN_LABEL,
								buf, &s)) {
					s--;
					memcpy(token->label, buf, s);
					memset(token->label + s, ' ',
					       sizeof(token->label) - s);
				}
			}
			if (!token->manufacturerID[0]) {
				s = sizeof(token->manufacturerID) + 1;
				if (!gnutls_pkcs11_obj_get_info(crt, GNUTLS_PKCS11_OBJ_TOKEN_MANUFACTURER,
								buf, &s)) {
					s--;
					memcpy(token->manufacturerID, buf, s);
					memset(token->manufacturerID + s, ' ',
					       sizeof(token->manufacturerID) - s);
				}
			}
			if (!token->model[0]) {
				s = sizeof(token->model) + 1;
				if (!gnutls_pkcs11_obj_get_info(crt, GNUTLS_PKCS11_OBJ_TOKEN_MODEL,
								buf, &s)) {
					s--;
					memcpy(token->model, buf, s);
					memset(token->model + s, ' ',
					       sizeof(token->model) - s);
				}
			}
			if (!token->serialNumber[0]) {
				s = sizeof(token->serialNumber) + 1;
				if (!gnutls_pkcs11_obj_get_info(crt, GNUTLS_PKCS11_OBJ_TOKEN_SERIAL,
								buf, &s)) {
					s--;
					memcpy(token->serialNumber, buf, s);
					memset(token->serialNumber + s, ' ',
					       sizeof(token->serialNumber) - s);
				}

			}

			free(key_url);
			key_url = NULL;
			if (!p11_kit_uri_format(uri, P11_KIT_URI_FOR_ANY, &key_url))
				err = gnutls_pkcs11_privkey_import_url(p11key, key_url, 0);
		key_err_uri:
			p11_kit_uri_free(uri);
		key_err_obj:
			gnutls_pkcs11_obj_deinit(crt);
		key_err:
			;
		}
		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Error importing PKCS#11 URL %s: %s\n"),
				     key_url, gnutls_strerror(err));
			gnutls_pkcs11_privkey_deinit(p11key);
			ret = -EIO;
			goto out;
		}

		err = gnutls_privkey_init(&pkey);
		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Error initialising private key structure: %s\n"),
				     gnutls_strerror(err));
			gnutls_pkcs11_privkey_deinit(p11key);
			ret = -EIO;
			goto out;
		}

		err = gnutls_privkey_import_pkcs11(pkey, p11key, GNUTLS_PRIVKEY_IMPORT_AUTO_RELEASE);
		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Error importing PKCS#11 key into private key structure: %s\n"),
				     gnutls_strerror(err));
			gnutls_pkcs11_privkey_deinit(p11key);
			ret = -EIO;
			goto out;
		}
#ifndef HAVE_GNUTLS_CERTIFICATE_SET_KEY
		/* This can be set now and doesn't need to be separately freed.
		   It goes with the pkey. This is a PITA; it would be better
		   if there was a way to get the p11key *back* from a privkey
		   that we *know* is based on one. In fact, since this is only
		   for GnuTLS 2.12 and we *know* the gnutls_privkey_st won't
		   ever change there, so we *could* do something evil... but
		   we won't :) */
		vpninfo->my_p11key = p11key;
#endif /* !SET_KEY */
		goto match_cert;
	}
#endif /* HAVE_P11KIT */

	/* OK, not a PKCS#11 key so it must be coming from a file... load the
	   file into memory, unless it's the same as the cert file and we
	   already loaded that. */
	if (!fdata.data || vpninfo->sslkey != vpninfo->cert) {
		gnutls_free(fdata.data);
		fdata.data = NULL;

		vpn_progress(vpninfo, PRG_DEBUG,
			     _("Using private key file %s\n"), vpninfo->sslkey);

		ret = load_datum(vpninfo, &fdata, vpninfo->sslkey);
		if (ret)
			goto out;
	}

	/* Is it a PEM file with a TPM key blob? */
	if (vpninfo->cert_type == CERT_TYPE_TPM ||
	    (vpninfo->cert_type == CERT_TYPE_UNKNOWN &&
	     strstr((char *)fdata.data, "-----BEGIN TSS KEY BLOB-----"))) {
#ifndef HAVE_TROUSERS
		vpn_progress(vpninfo, PRG_ERR,
			     _("This version of OpenConnect was built without TPM support\n"));
		return -EINVAL;
#else
		ret = load_tpm_key(vpninfo, &fdata, &pkey, &pkey_sig);
		if (ret)
			goto out;

		goto match_cert;
#endif
	}

	/* OK, try other PEM files... */
	gnutls_x509_privkey_init(&key);
	if ((pem_header = strstr((char *)fdata.data, "-----BEGIN RSA PRIVATE KEY-----")) ||
	    (pem_header = strstr((char *)fdata.data, "-----BEGIN DSA PRIVATE KEY-----")) ||
	    (pem_header = strstr((char *)fdata.data, "-----BEGIN EC PRIVATE KEY-----"))) {
		/* PKCS#1 files, including OpenSSL's odd encrypted version */
		char type = pem_header[11];
		char *p = strchr(pem_header, '\n');
		if (!p) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to interpret PEM file\n"));
			ret = -EINVAL;
			goto out;
		}
		while (*p == '\n' || *p == '\r')
			p++;

		if (!strncmp(p, "Proc-Type: 4,ENCRYPTED", 22)) {
			p += 22;
			while (*p == '\n' || *p == '\r')
				p++;
			ret = import_openssl_pem(vpninfo, key, type, p,
						 fdata.size - (p - (char *)fdata.data));
			if (ret)
				goto out;
		} else {
			err = gnutls_x509_privkey_import(key, &fdata, GNUTLS_X509_FMT_PEM);
			if (err) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Failed to load PKCS#1 private key: %s\n"),
					     gnutls_strerror(err));
				ret = -EINVAL;
				goto out;
			}
		}
	} else if (strstr((char *)fdata.data, "-----BEGIN PRIVATE KEY-----")) {
		/* Unencrypted PKCS#8 */
		err = gnutls_x509_privkey_import_pkcs8(key, &fdata,
						       GNUTLS_X509_FMT_PEM,
						       NULL, GNUTLS_PKCS_PLAIN);
		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
					     _("Failed to load private key as PKCS#8: %s\n"),
					     gnutls_strerror(err));
			ret = -EINVAL;
			goto out;
		}
	} else if (strstr((char *)fdata.data, "-----BEGIN ENCRYPTED PRIVATE KEY-----")) {
		/* Encrypted PKCS#8 */
		char *pass = vpninfo->cert_password;

		while ((err = gnutls_x509_privkey_import_pkcs8(key, &fdata,
							       GNUTLS_X509_FMT_PEM,
							       pass?:"", 0))) {
			if (err != GNUTLS_E_DECRYPTION_FAILED) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Failed to load private key as PKCS#8: %s\n"),
					     gnutls_strerror(err));
				ret = -EINVAL;
				goto out;
			}
			vpninfo->cert_password = NULL;
			if (pass) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Failed to decrypt PKCS#8 certificate file\n"));
				free(pass);
			}
			err = request_passphrase(vpninfo, "openconnect_pem",
						 &pass, _("Enter PEM pass phrase:"));
			if (err) {
				ret = -EINVAL;
				goto out;
			}
		}
		free(pass);
		vpninfo->cert_password = NULL;
	} else {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to determine type of private key %s\n"),
			     vpninfo->sslkey);
		ret = -EINVAL;
		goto out;
	}

	/* Now attempt to make sure we use the *correct* certificate, to match
	   the key. Since we have a software key, we can easily query it and
	   compare its key_id with each certificate till we find a match. */
	err = gnutls_x509_privkey_get_key_id(key, 0, key_id, &key_id_size);
	if (err) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to get key ID: %s\n"),
			     gnutls_strerror(err));
		ret = -EINVAL;
		goto out;
	}
	/* If extra_certs[] is NULL, we have one candidate in 'cert' to check. */
	for (i = 0; i < (extra_certs ? nr_extra_certs : 1); i++) {
		unsigned char cert_id[20];
		size_t cert_id_size = sizeof(cert_id);

		err = gnutls_x509_crt_get_key_id(extra_certs ? extra_certs[i] : cert, 0, cert_id, &cert_id_size);
		if (err)
			continue;

		if (cert_id_size == key_id_size && !memcmp(cert_id, key_id, key_id_size)) {
			if (extra_certs) {
				cert = extra_certs[i];
				extra_certs[i] = NULL;
			}
			goto got_key;
		}
	}
	/* There's no pkey (there's an x509 key), so even if p11-kit or trousers is
	   enabled we'll fall straight through the bit at match_cert: below, and go
	   directly to the bit where it prints the 'no match found' error and exits. */

#if defined(HAVE_P11KIT) || defined(HAVE_TROUSERS)
 match_cert:
	/* If we have a privkey from PKCS#11 or TPM, we can't do the simple comparison
	   of key ID that we do for software keys to find which certificate is a
	   match. So sign some dummy data and then check the signature against each
	   of the available certificates until we find the right one. */
	if (pkey) {
		/* The TPM code may have already signed it, to test authorisation. We
		   only sign here for PKCS#11 keys, in which case fdata might be
		   empty too so point it at dummy data. */
		if (!pkey_sig.data) {
			if (!fdata.data) {
				fdata.data = dummy_hash_data;
				fdata.size = 20;
			}

			err = sign_dummy_data(vpninfo, pkey, &fdata, &pkey_sig);
			if (err) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Error signing test data with private key: %s\n"),
					     gnutls_strerror(err));
				ret = -EINVAL;
				goto out;
			}
		}

		/* If extra_certs[] is NULL, we have one candidate in 'cert' to check. */
		for (i = 0; i < (extra_certs ? nr_extra_certs : 1); i++) {
			gnutls_pubkey_t pubkey;

			gnutls_pubkey_init(&pubkey);
			err = gnutls_pubkey_import_x509(pubkey, extra_certs ? extra_certs[i] : cert, 0);
			if (err) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Error validating signature against certificate: %s\n"),
					     gnutls_strerror(err));
				/* We'll probably fail shortly if we don't find it. */
				gnutls_pubkey_deinit(pubkey);
				continue;
			}
			err = verify_signed_data(pubkey, pkey, &fdata, &pkey_sig);
			gnutls_pubkey_deinit(pubkey);

			if (err >= 0) {
				if (extra_certs) {
					cert = extra_certs[i];
					extra_certs[i] = NULL;
				}
				gnutls_free(pkey_sig.data);
				goto got_key;
			}
		}
		gnutls_free(pkey_sig.data);
	}
#endif /* P11KIT || TROUSERS */

	/* We shouldn't reach this. It means that we didn't find *any* matching cert */
	vpn_progress(vpninfo, PRG_ERR,
		     _("No SSL certificate found to match private key\n"));
	ret = -EINVAL;
	goto out;

	/********************************************************************/
 got_key:
	/* Now we have a key in either 'key' or 'pkey', a matching cert in 'cert',
	   and potentially a list of other certs in 'extra_certs[]'. If we loaded
	   a PKCS#12 file we may have a trust chain in 'supporting_certs[]' too. */
	check_certificate_expiry(vpninfo, cert);
	get_cert_name(cert, name, sizeof(name));
	vpn_progress(vpninfo, PRG_INFO, _("Using client certificate '%s'\n"),
		     name);

	if (crl) {
		err = gnutls_certificate_set_x509_crl(vpninfo->https_cred, &crl, 1);
		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Setting certificate recovation list failed: %s\n"),
				     gnutls_strerror(err));
			ret = -EINVAL;
			goto out;
		}
	}

	/* OpenSSL has problems with certificate chains — if there are
	   multiple certs with the same name, it doesn't necessarily
	   choose the _right_ one. (RT#1942)
	   Pick the right ones for ourselves and add them manually. */

	/* We may have already got a bunch of certs from PKCS#12
	   file. Remember how many need to be freed when we're done,
	   since we'll expand the supporting_certs array with more
	   from the cafile and extra_certs[] array if we can, and
	   those extra certs must not be freed (twice). */
	if (!nr_supporting_certs) {
		supporting_certs = gnutls_malloc(sizeof(*supporting_certs));
		if (!supporting_certs) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to allocate memory for certificate\n"));
			ret = -ENOMEM;
			goto out;
		}
		supporting_certs[0] = cert;
		nr_supporting_certs = 1;

		free_supporting_certs = gnutls_malloc(1);
		if (!free_supporting_certs) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to allocate memory for certificate\n"));
			ret = -ENOMEM;
			goto out;
		}
		free_supporting_certs[0] = 1;
	}
	last_cert = supporting_certs[nr_supporting_certs-1];

	while (1) {
		uint8_t free_issuer;
		gnutls_x509_crt_t issuer;
		void *tmp;

		for (i = 0; i < nr_extra_certs; i++) {
			if (extra_certs[i] &&
			    gnutls_x509_crt_check_issuer(last_cert, extra_certs[i]) &&
			    !check_issuer_sanity(last_cert, extra_certs[i]))
				break;
		}

		if (i < nr_extra_certs) {
			/* We found the next cert in the chain in extra_certs[] */
			issuer = extra_certs[i];
			extra_certs[i] = NULL;
			free_issuer = 1;
		} else {
			/* Look for it in the system trust cafile too. */
			err = gnutls_certificate_get_issuer(vpninfo->https_cred,
							    last_cert, &issuer, 0);
			/* The check_issuer_sanity() function works fine as a workaround where
			   it was used above, but when gnutls_certificate_get_issuer() returns
			   a bogus cert, there's nothing we can do to fix it up. We don't get
			   to iterate over all the available certs like we can over our own
			   list. */
			if (!err && check_issuer_sanity(last_cert, issuer)) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("WARNING: GnuTLS returned incorrect issuer certs; authentication may fail!\n"));
				break;
			}
			free_issuer = 0;

#if defined(HAVE_P11KIT) && defined(HAVE_GNUTLS_PKCS11_GET_RAW_ISSUER)
			if (err && cert_is_p11) {
				gnutls_datum_t t;

				err = gnutls_pkcs11_get_raw_issuer(cert_url, last_cert, &t, GNUTLS_X509_FMT_DER, 0);
				if (!err) {
					err = gnutls_x509_crt_init(&issuer);
					if (!err) {
						err = gnutls_x509_crt_import(issuer, &t, GNUTLS_X509_FMT_DER);
						if (err)
							gnutls_x509_crt_deinit(issuer);
						else
							free_issuer = 1;
					}
					gnutls_free(t.data);
				}
				if (err) {
					vpn_progress(vpninfo, PRG_ERR,
						     "Got no issuer from PKCS#11\n");
				} else {
					get_cert_name(issuer, name, sizeof(name));

					vpn_progress(vpninfo, PRG_ERR,
						     _("Got next CA '%s' from PKCS11\n"), name);
				}
			}
#endif
			if (err)
				break;

		}

		if (gnutls_x509_crt_check_issuer(issuer, issuer)) {
			/* Don't actually include the root CA. If they don't already trust it,
			   then handing it to them isn't going to help. But don't omit the
			   original certificate if it's self-signed. */
			if (free_issuer)
				gnutls_x509_crt_deinit(issuer);
			break;
		}

		/* OK, we found a new cert to add to our chain. */
		tmp = supporting_certs;
		supporting_certs = gnutls_realloc(supporting_certs,
						  sizeof(cert) * (nr_supporting_certs+1));
		if (!supporting_certs) {
			supporting_certs = tmp;
		realloc_failed:
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to allocate memory for supporting certificates\n"));
			if (free_issuer)
				gnutls_x509_crt_deinit(issuer);
			break;
		}

		tmp = free_supporting_certs;
		free_supporting_certs = gnutls_realloc(free_supporting_certs, nr_supporting_certs+1);
		if (!free_supporting_certs) {
			free_supporting_certs = tmp;
			goto realloc_failed;
		}

		/* Append the new one */
		supporting_certs[nr_supporting_certs] = issuer;
		free_supporting_certs[nr_supporting_certs] = free_issuer;
		nr_supporting_certs++;
		last_cert = issuer;
	}
	for (i = 1; i < nr_supporting_certs; i++) {
		get_cert_name(supporting_certs[i], name, sizeof(name));

		vpn_progress(vpninfo, PRG_DEBUG,
			     _("Adding supporting CA '%s'\n"), name);
	}

	/* OK, now we've checked the cert expiry and warned the user if it's
	   going to expire soon, and we've built up as much of a trust chain
	   in supporting_certs[] as we can find, to help the server work around
	   OpenSSL RT#1942. Set up the GnuTLS credentials with the appropriate
	   key and certs. GnuTLS makes us do this differently for X509 privkeys
	   vs. TPM/PKCS#11 "generic" privkeys, and the latter is particularly
	   'fun' for GnuTLS 2.12... */
#if defined(HAVE_P11KIT) || defined(HAVE_TROUSERS)
	if (pkey) {
		err = assign_privkey(vpninfo, pkey,
				     supporting_certs,
				     nr_supporting_certs,
				     free_supporting_certs);
		if (!err) {
			pkey = NULL; /* we gave it away, and potentially also some
					of extra_certs[] may have been zeroed. */
		}
	} else
#endif /* P11KIT || TROUSERS */
		err = gnutls_certificate_set_x509_key(vpninfo->https_cred,
						      supporting_certs,
						      nr_supporting_certs, key);

	if (err) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Setting certificate failed: %s\n"),
			     gnutls_strerror(err));
		ret = -EIO;
	} else
		ret = 0;
 out:
	if (crl)
		gnutls_x509_crl_deinit(crl);
	if (key)
		gnutls_x509_privkey_deinit(key);
	if (supporting_certs) {
		for (i = 0; i < nr_supporting_certs; i++) {
			/* We get here in an error case with !free_supporting_certs
			   and should free them all in that case */
			if (!free_supporting_certs || free_supporting_certs[i])
				gnutls_x509_crt_deinit(supporting_certs[i]);
		}
		gnutls_free(supporting_certs);
		gnutls_free(free_supporting_certs);
	} else if (cert) {
		/* Not if supporting_certs. It's supporting_certs[0] then and
		   was already freed. */
		gnutls_x509_crt_deinit(cert);
	}
	for (i = 0; i < nr_extra_certs; i++) {
		if (extra_certs[i])
			gnutls_x509_crt_deinit(extra_certs[i]);
	}
	gnutls_free(extra_certs);

#if defined(HAVE_P11KIT) || defined(HAVE_TROUSERS)
	if (pkey && pkey != OPENCONNECT_TPM_PKEY)
		gnutls_privkey_deinit(pkey);
	/* If we support arbitrary privkeys, we might have abused fdata.data
	   just to point to something to hash. Don't free it in that case! */
	if (fdata.data != dummy_hash_data)
#endif
		gnutls_free(fdata.data);

#ifdef HAVE_P11KIT
	if (cert_url != vpninfo->cert)
		free(cert_url);
	if (key_url != vpninfo->sslkey)
		free(key_url);
#endif
	return ret;
}