static void
print_escaped (stream, str)
     FILE *stream;
     const char *str;
{
  putc ('"', stream);
  for (; *str != '\0'; str++)
    if (*str == '\n')
      {
	fputs ("\\n\"", stream);
	if (str[1] == '\0')
	  return;
	fputs ("\n\"", stream);
      }
    else
      {
	if (*str == '"' || *str == '\\')
	  putc ('\\', stream);
	putc (*str, stream);
      }
  putc ('"', stream);
}
