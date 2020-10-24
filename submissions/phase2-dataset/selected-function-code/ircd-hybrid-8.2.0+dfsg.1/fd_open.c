void
fd_open(fde_t *F, int fd, int is_socket, const char *desc)
{
  unsigned int hashv = hash_fd(fd);
  assert(fd >= 0);

  F->fd = fd;
  F->comm_index = -1;

  if (desc)
    strlcpy(F->desc, desc, sizeof(F->desc));

  /* Note: normally we'd have to clear the other flags,
   * but currently F is always cleared before calling us.. */
  F->flags.open = 1;
  F->flags.is_socket = is_socket;
  F->hnext = fd_hash[hashv];
  fd_hash[hashv] = F;

  number_fd++;
}
