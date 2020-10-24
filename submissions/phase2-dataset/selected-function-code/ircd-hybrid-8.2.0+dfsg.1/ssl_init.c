ssl_init(void)
{
#ifdef HAVE_LIBCRYPTO
  const unsigned char session_id[] = "ircd-hybrid";

  SSL_load_error_strings();
  SSLeay_add_ssl_algorithms();

  if ((ConfigServerInfo.server_ctx = SSL_CTX_new(SSLv23_server_method())) == NULL)
  {
    const char *s = ERR_lib_error_string(ERR_get_error());

    fprintf(stderr, "ERROR: Could not initialize the SSL Server context -- %s\n", s);
    ilog(LOG_TYPE_IRCD, "ERROR: Could not initialize the SSL Server context -- %s\n", s);
    exit(EXIT_FAILURE);
  }

  SSL_CTX_set_options(ConfigServerInfo.server_ctx, SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3);
  SSL_CTX_set_options(ConfigServerInfo.server_ctx, SSL_OP_SINGLE_DH_USE);
  SSL_CTX_set_verify(ConfigServerInfo.server_ctx, SSL_VERIFY_PEER|SSL_VERIFY_CLIENT_ONCE,
                     always_accept_verify_cb);
  SSL_CTX_set_session_id_context(ConfigServerInfo.server_ctx, session_id, sizeof(session_id) - 1);

#if OPENSSL_VERSION_NUMBER >= 0x1000005FL && !defined(OPENSSL_NO_ECDH)
  {
    EC_KEY *key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);

    if (key)
    {
      SSL_CTX_set_tmp_ecdh(ConfigServerInfo.server_ctx, key);
      EC_KEY_free(key);
    }
  }

  SSL_CTX_set_options(ConfigServerInfo.server_ctx, SSL_OP_SINGLE_ECDH_USE);
#endif

  if ((ConfigServerInfo.client_ctx = SSL_CTX_new(SSLv23_client_method())) == NULL)
  {
    const char *s = ERR_lib_error_string(ERR_get_error());

    fprintf(stderr, "ERROR: Could not initialize the SSL Client context -- %s\n", s);
    ilog(LOG_TYPE_IRCD, "ERROR: Could not initialize the SSL Client context -- %s\n", s);
    exit(EXIT_FAILURE);
  }

  SSL_CTX_set_options(ConfigServerInfo.client_ctx, SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3);
  SSL_CTX_set_options(ConfigServerInfo.client_ctx, SSL_OP_SINGLE_DH_USE);
  SSL_CTX_set_verify(ConfigServerInfo.client_ctx, SSL_VERIFY_PEER|SSL_VERIFY_CLIENT_ONCE,
                     always_accept_verify_cb);
#endif /* HAVE_LIBCRYPTO */
}
