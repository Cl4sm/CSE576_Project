static void
add_one_to_uid(unsigned int i)
{
  if (i != IRC_MAXSID)  /* Not reached server SID portion yet? */
  {
    if (new_uid[i] == 'Z')
      new_uid[i] = '0';
    else if (new_uid[i] == '9')
    {
      new_uid[i] = 'A';
      add_one_to_uid(i - 1);
    }
    else
      ++new_uid[i];
  }
  else
  {
    /* NOTE: if IRC_MAXUID != 6, this will have to be rewritten */
    if (new_uid[i] == 'Z')
      memcpy(new_uid + IRC_MAXSID, "AAAAAA", IRC_MAXUID);
    else
      ++new_uid[i];
  }
}
