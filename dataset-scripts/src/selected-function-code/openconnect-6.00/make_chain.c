static int make_chain(gnutls_x509_crt_t **chain, unsigned int *chain_len,
                      gnutls_x509_crt_t **extra_certs, unsigned int *extra_certs_len)
{
unsigned int i;

  if (*chain_len != 1)
    return gnutls_assert_val(GNUTLS_E_INVALID_REQUEST);
  
  i = 0;
  while(i<*extra_certs_len)
    {
      /* if it is an issuer but not a self-signed one */
      if (gnutls_x509_crt_check_issuer((*chain)[*chain_len - 1], (*extra_certs)[i]) != 0 &&
          gnutls_x509_crt_check_issuer((*extra_certs)[i], (*extra_certs)[i]) == 0)
        {
           void *tmp = *chain;
           *chain = gnutls_realloc (*chain, sizeof((*chain)[0]) *
                                                     ++(*chain_len));
           if (*chain == NULL)
             {
               gnutls_assert();
               gnutls_free(tmp);
               return GNUTLS_E_MEMORY_ERROR;
             }
           (*chain)[*chain_len - 1] = (*extra_certs)[i];
           
           (*extra_certs)[i] = (*extra_certs)[*extra_certs_len-1];
           (*extra_certs_len)--;

           i=0;
           continue;
        }
      i++;
    }
  return 0;
}