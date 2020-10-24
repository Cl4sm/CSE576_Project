char *
deja_dup_decoded_uri_encode_uri (DejaDupDecodedUri *decoded, gboolean allow_utf8)
{
  GString *uri;

  uri = g_string_new (NULL);

  g_string_append (uri, decoded->scheme);
  g_string_append (uri, "://");

  if (decoded->host != NULL)
    {
      if (decoded->userinfo)
        {
          /* userinfo    = *( unreserved / pct-encoded / sub-delims / ":" ) */
          g_string_append_uri_escaped (uri, decoded->userinfo,
                                       G_URI_RESERVED_CHARS_ALLOWED_IN_USERINFO, allow_utf8);
          g_string_append_c (uri, '@');
        }
      
      g_string_append_uri_escaped (uri, decoded->host,
                                   /* Allowed unescaped in hostname / ip address */
                                   G_URI_RESERVED_CHARS_SUBCOMPONENT_DELIMITERS ":[]" ,
                                   allow_utf8);
      
      if (decoded->port != -1)
        {
          g_string_append_c (uri, ':');
          g_string_append_printf (uri, "%d", decoded->port);
        }
    }

  g_string_append_uri_escaped (uri, decoded->path, G_URI_RESERVED_CHARS_ALLOWED_IN_PATH, allow_utf8);
  
  if (decoded->query)
    {
      g_string_append_c (uri, '?');
      g_string_append (uri, decoded->query);
    }
    
  if (decoded->fragment)
    {
      g_string_append_c (uri, '#');
      g_string_append (uri, decoded->fragment);
    }

  return g_string_free (uri, FALSE);
}