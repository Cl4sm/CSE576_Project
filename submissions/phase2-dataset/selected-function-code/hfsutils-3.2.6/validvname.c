static
int validvname(const char *name)
{
  int len;

  len = strlen(name);
  if (len < 1)
    ERROR(EINVAL, "volume name cannot be empty");
  else if (len > HFS_MAX_VLEN)
    ERROR(ENAMETOOLONG,
	  "volume name can be at most " STR(HFS_MAX_VLEN) " chars");

  if (strchr(name, ':'))
    ERROR(EINVAL, "volume name may not contain colons");

  return 1;

fail:
  return 0;
}
