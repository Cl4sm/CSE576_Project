fd_close(fde_t *F)
{
  unsigned int hashv = hash_fd(F->fd);

  if (F == fd_next_in_loop)
    fd_next_in_loop = F->hnext;

  if (F->flags.is_socket)
    comm_setselect(F, COMM_SELECT_WRITE | COMM_SELECT_READ, NULL, NULL, 0);

  delete_resolver_queries(F);

#ifdef HAVE_LIBCRYPTO
  if (F->ssl)
    SSL_free(F->ssl);
#endif

  if (fd_hash[hashv] == F)
    fd_hash[hashv] = F->hnext;
  else
  {
    fde_t *prev;

    /* let it core if not found */
    for (prev = fd_hash[hashv]; prev->hnext != F; prev = prev->hnext)
      ;
    prev->hnext = F->hnext;
  }

  /* Unlike squid, we're actually closing the FD here! -- adrian */
  close(F->fd);
  number_fd--;

  memset(F, 0, sizeof(fde_t));
}
