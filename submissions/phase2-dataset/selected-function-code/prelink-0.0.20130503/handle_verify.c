static int
handle_verify (int fd, const char *filename)
{
  off_t off;
  size_t cnt;
  struct stat64 st;

  if (fstat64 (fd, &st) < 0)
    {
      error (0, errno, "%s: couldn't fstat temporary file", filename);
      return 1;
    }

  if (verify_method == VERIFY_CONTENT)
    {
      off = 0;
      if (send_file (1, fd, &off, st.st_size) != st.st_size)
	{
	  error (0, errno, "Couldn't write file to standard output");
	  return 1;
	}
    }
  else if (verify_method == VERIFY_MD5)
    {
      struct md5_ctx ctx;
      unsigned char bin_buffer[16];

      md5_init_ctx (&ctx);
      if (checksum_file (fd, st.st_size,
			 (void (*) (const void *, size_t, void *))
			 md5_process_bytes, &ctx))
	{
	  error (0, errno, "%s: Couldn't read temporary file", filename);
	  return 1;
	}

      md5_finish_ctx (&ctx, bin_buffer);
      for (cnt = 0; cnt < 16; ++cnt)
	printf ("%02x", bin_buffer[cnt]);
      printf ("  %s\n", filename);
    }
  else if (verify_method == VERIFY_SHA)
    {
      struct sha_ctx ctx;
      unsigned char bin_buffer[20];

      sha_init_ctx (&ctx);
      if (checksum_file (fd, st.st_size,
			 (void (*) (const void *, size_t, void *))
			 sha_process_bytes, &ctx))
	{
	  error (0, errno, "%s: Couldn't read temporary file", filename);
	  return 1;
	}

      sha_finish_ctx (&ctx, bin_buffer);
      for (cnt = 0; cnt < 20; ++cnt)
	printf ("%02x", bin_buffer[cnt]);
      printf ("  %s\n", filename);
    }
  return 0;
}
