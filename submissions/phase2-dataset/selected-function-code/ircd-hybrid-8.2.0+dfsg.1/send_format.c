static void
send_format(struct dbuf_block *buffer, const char *pattern, va_list args)
{
  /*
   * from rfc1459
   *
   * IRC messages are always lines of characters terminated with a CR-LF
   * (Carriage Return - Line Feed) pair, and these messages shall not
   * exceed 512 characters in length,  counting all characters
   * including the trailing CR-LF.
   * Thus, there are 510 characters maximum allowed
   * for the command and its parameters.  There is no provision for
   * continuation message lines.  See section 7 for more details about
   * current implementations.
   */
  dbuf_put_args(buffer, pattern, args);

  if (buffer->size > sizeof(buffer->data) - 2)
    buffer->size = sizeof(buffer->data) - 2;

  buffer->data[buffer->size++] = '\r';
  buffer->data[buffer->size++] = '\n';
}
