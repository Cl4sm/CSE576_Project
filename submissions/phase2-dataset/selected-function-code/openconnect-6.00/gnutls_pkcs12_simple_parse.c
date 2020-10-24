int
gnutls_pkcs12_simple_parse (gnutls_pkcs12_t p12,
                     const char *password,
                     gnutls_x509_privkey_t * key,
                     gnutls_x509_crt_t ** chain,
                     unsigned int * chain_len,
                     gnutls_x509_crt_t ** extra_certs,
                     unsigned int * extra_certs_len,
                     gnutls_x509_crl_t * crl,
                     unsigned int flags)
{
  gnutls_pkcs12_bag_t bag = NULL;
  gnutls_x509_crt_t *_extra_certs = NULL;
  unsigned int _extra_certs_len = 0;
  gnutls_x509_crt_t *_chain = NULL;
  unsigned int _chain_len = 0;
  int idx = 0;
  int ret;
  size_t cert_id_size = 0;
  size_t key_id_size = 0;
  opaque cert_id[20];
  opaque key_id[20];
  int privkey_ok = 0;

  *key = NULL;
  
  if (crl)
    *crl = NULL;

  /* find the first private key */
  for (;;)
    {
      int elements_in_bag;
      int i;

      ret = gnutls_pkcs12_bag_init (&bag);
      if (ret < 0)
        {
          bag = NULL;
          gnutls_assert ();
          goto done;
        }

      ret = gnutls_pkcs12_get_bag (p12, idx, bag);
      if (ret == GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE)
        break;
      if (ret < 0)
        {
          gnutls_assert ();
          goto done;
        }

      ret = gnutls_pkcs12_bag_get_type (bag, 0);
      if (ret < 0)
        {
          gnutls_assert ();
          goto done;
        }

      if (ret == GNUTLS_BAG_ENCRYPTED)
        {
          if (password == NULL)
            {
              ret = gnutls_assert_val(GNUTLS_E_DECRYPTION_FAILED);
              goto done;
            }

          ret = gnutls_pkcs12_bag_decrypt (bag, password);
          if (ret < 0)
            {
              gnutls_assert ();
              goto done;
            }
        }

      elements_in_bag = gnutls_pkcs12_bag_get_count (bag);
      if (elements_in_bag < 0)
        {
          gnutls_assert ();
          goto done;
        }

      for (i = 0; i < elements_in_bag; i++)
        {
          int type;
          gnutls_datum_t data;

          type = gnutls_pkcs12_bag_get_type (bag, i);
          if (type < 0)
            {
              gnutls_assert ();
              goto done;
            }

          ret = gnutls_pkcs12_bag_get_data (bag, i, &data);
          if (ret < 0)
            {
              gnutls_assert ();
              goto done;
            }

          switch (type)
            {
            case GNUTLS_BAG_PKCS8_ENCRYPTED_KEY:
              if (password == NULL)
                {
                  ret = gnutls_assert_val(GNUTLS_E_DECRYPTION_FAILED);
                  goto done;
                }

            case GNUTLS_BAG_PKCS8_KEY:
              if (*key != NULL) /* too simple to continue */
                {
                  gnutls_assert ();
                  break;
                }

              ret = gnutls_x509_privkey_init (key);
              if (ret < 0)
                {
                  gnutls_assert ();
                  goto done;
                }

              ret = gnutls_x509_privkey_import_pkcs8
                (*key, &data, GNUTLS_X509_FMT_DER, password,
                 type == GNUTLS_BAG_PKCS8_KEY ? GNUTLS_PKCS_PLAIN : 0);
              if (ret < 0)
                {
                  gnutls_assert ();
                  gnutls_x509_privkey_deinit (*key);
                  goto done;
                }

              key_id_size = sizeof (key_id);
              ret =
                gnutls_x509_privkey_get_key_id (*key, 0, key_id,
                                                &key_id_size);
              if (ret < 0)
                {
                  gnutls_assert ();
                  gnutls_x509_privkey_deinit (*key);
                  goto done;
                }

              privkey_ok = 1;   /* break */
              break;
            default:
              break;
            }
        }

      idx++;
      gnutls_pkcs12_bag_deinit (bag);

      if (privkey_ok != 0)      /* private key was found */
        break;
    }

  if (privkey_ok == 0)          /* no private key */
    {
      gnutls_assert ();
      return GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE;
    }

  /* now find the corresponding certificate 
   */
  idx = 0;
  bag = NULL;
  for (;;)
    {
      int elements_in_bag;
      int i;

      ret = gnutls_pkcs12_bag_init (&bag);
      if (ret < 0)
        {
          bag = NULL;
          gnutls_assert ();
          goto done;
        }

      ret = gnutls_pkcs12_get_bag (p12, idx, bag);
      if (ret == GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE)
        break;
      if (ret < 0)
        {
          gnutls_assert ();
          goto done;
        }

      ret = gnutls_pkcs12_bag_get_type (bag, 0);
      if (ret < 0)
        {
          gnutls_assert ();
          goto done;
        }

      if (ret == GNUTLS_BAG_ENCRYPTED)
        {
          ret = gnutls_pkcs12_bag_decrypt (bag, password);
          if (ret < 0)
            {
              gnutls_assert ();
              goto done;
            }
        }

      elements_in_bag = gnutls_pkcs12_bag_get_count (bag);
      if (elements_in_bag < 0)
        {
          gnutls_assert ();
          goto done;
        }

      for (i = 0; i < elements_in_bag; i++)
        {
          int type;
          gnutls_datum_t data;
          gnutls_x509_crt_t this_cert;

          type = gnutls_pkcs12_bag_get_type (bag, i);
          if (type < 0)
            {
              gnutls_assert ();
              goto done;
            }

          ret = gnutls_pkcs12_bag_get_data (bag, i, &data);
          if (ret < 0)
            {
              gnutls_assert ();
              goto done;
            }

          switch (type)
            {
            case GNUTLS_BAG_CERTIFICATE:
              ret = gnutls_x509_crt_init (&this_cert);
              if (ret < 0)
                {
                  gnutls_assert ();
                  goto done;
                }

              ret =
                gnutls_x509_crt_import (this_cert, &data, GNUTLS_X509_FMT_DER);
              if (ret < 0)
                {
                  gnutls_assert ();
                  gnutls_x509_crt_deinit (this_cert);
                  goto done;
                }

              /* check if the key id match */
              cert_id_size = sizeof (cert_id);
              ret =
                gnutls_x509_crt_get_key_id (this_cert, 0, cert_id, &cert_id_size);
              if (ret < 0)
                {
                  gnutls_assert ();
                  gnutls_x509_crt_deinit (this_cert);
                  goto done;
                }

              if (memcmp (cert_id, key_id, cert_id_size) != 0)
                { /* they don't match - skip the certificate */
                  if (extra_certs)
                    {
                      void *tmp = _extra_certs;
                      _extra_certs = gnutls_realloc (_extra_certs,
                                                     sizeof(_extra_certs[0]) *
                                                     ++_extra_certs_len);
                      if (!_extra_certs)
                        {
                          gnutls_assert ();
                          gnutls_free(tmp);
                          ret = GNUTLS_E_MEMORY_ERROR;
                          goto done;
                        }
                      _extra_certs[_extra_certs_len - 1] = this_cert;
                      this_cert = NULL;
                    }
                  else
                    {
                       gnutls_x509_crt_deinit (this_cert);
                    }
                }
              else
                {
                  if (_chain_len == 0)
                    {
                      _chain = gnutls_malloc (sizeof(_chain[0]) * (++_chain_len));
                      if (!_chain)
                        {
                          gnutls_assert ();
                          ret = GNUTLS_E_MEMORY_ERROR;
                          goto done;
                        }
                      _chain[_chain_len - 1] = this_cert;
                      this_cert = NULL;
                    }
                  else
                    {
                       gnutls_x509_crt_deinit (this_cert);
                    }
                }
              break;

            case GNUTLS_BAG_CRL:
              if (crl == NULL || *crl != NULL)
                {
                  gnutls_assert ();
                  break;
                }

              ret = gnutls_x509_crl_init (crl);
              if (ret < 0)
                {
                  gnutls_assert ();
                  goto done;
                }

              ret = gnutls_x509_crl_import (*crl, &data, GNUTLS_X509_FMT_DER);
              if (ret < 0)
                {
                  gnutls_assert ();
                  gnutls_x509_crl_deinit (*crl);
                  goto done;
                }
              break;

            case GNUTLS_BAG_ENCRYPTED:
              /* XXX Bother to recurse one level down?  Unlikely to
                 use the same password anyway. */
            case GNUTLS_BAG_EMPTY:
            default:
              break;
            }
        }

      idx++;
      gnutls_pkcs12_bag_deinit (bag);
    }

  if (_chain_len != 1)
    {
      ret = GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE;
      goto done;
    }

  ret = make_chain(&_chain, &_chain_len, &_extra_certs, &_extra_certs_len);
  if (ret < 0)
    {
      gnutls_assert();
      goto done;
    }

  ret = 0;

done:
  if (bag)
    gnutls_pkcs12_bag_deinit (bag);

  if (ret < 0)
    {
      if (*key)
        gnutls_x509_privkey_deinit(*key);
      if (_extra_certs_len && _extra_certs != NULL)
        {
          unsigned int i;
          for (i = 0; i < _extra_certs_len; i++)
            gnutls_x509_crt_deinit(_extra_certs[i]);
          gnutls_free(_extra_certs);
        }
      if (_chain_len && _chain != NULL)
        {
          unsigned int i;
          for (i = 0; i < _chain_len; i++)
            gnutls_x509_crt_deinit(_chain[i]);
          gnutls_free(_chain);
        }
    }
  else 
    {
      if (extra_certs) 
        {
          *extra_certs = _extra_certs;
          *extra_certs_len = _extra_certs_len;
        }
      
      *chain = _chain;
      *chain_len = _chain_len;
    }

  return ret;
}