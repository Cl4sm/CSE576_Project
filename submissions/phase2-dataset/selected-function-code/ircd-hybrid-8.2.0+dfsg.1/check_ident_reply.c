check_ident_reply(char *reply)
{
  char *token = NULL, *end = NULL;
  char *vector[USERID_TOKEN_COUNT];
  int count = token_vector(reply, ':', vector, USERID_TOKEN_COUNT);

  if (USERID_TOKEN_COUNT != count)
    return NULL;

  /*
   * Second token is the reply type
   */
  token = vector[IDENT_REPLY_TYPE];

  if (EmptyString(token))
    return NULL;

  while (IsSpace(*token))
    ++token;

  if (strncmp(token, "USERID", 6))
    return NULL;

  /*
   * Third token is the os type
   */
  token = vector[IDENT_OS_TYPE];

  if (EmptyString(token))
    return NULL;

  while (IsSpace(*token))
   ++token;

  /*
   * Unless "OTHER" is specified as the operating system type, the server
   * is expected to return the "normal" user identification of the owner
   * of this connection. "Normal" in this context may be taken to mean a
   * string of characters which uniquely identifies the connection owner
   * such as a user identifier assigned by the system administrator and
   * used by such user as a mail identifier, or as the "user" part of a
   * user/password pair used to gain access to system resources. When an
   * operating system is specified (e.g., anything but "OTHER"), the user
   * identifier is expected to be in a more or less immediately useful
   * form - e.g., something that could be used as an argument to "finger"
   * or as a mail address.
   */
  if (!strncmp(token, "OTHER", 5))
    return NULL;

  /*
   * Fourth token is the username
   */
  token = vector[IDENT_INFO];

  if (EmptyString(token))
    return NULL;

  while (IsSpace(*token))
    ++token;

  while (*token == '~' || *token == '^')
    ++token;

  /*
   * Look for the end of the username, terminators are '\0, @, <SPACE>, :'
   */
  for (end = token; *end; ++end)
    if (IsSpace(*end) || '@' == *end || ':' == *end)
      break;
  *end = '\0';

  return token;
}
