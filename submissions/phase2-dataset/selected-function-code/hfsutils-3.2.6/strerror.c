{
  static char str[14 + CHARLEN(int) + 1];

# ifdef HAVE_SYS_ERRLIST

  if (errnum >= 0 && errnum < sys_nerr)
    return sys_errlist[errnum];

# else

  switch (errnum)
    {
    case ENOENT:	return "No such file or directory";
    case EIO:		return "I/O error";
    case ENOMEM:	return "Out of memory";
    case EEXIST:	return "File exists";
    case ENOTDIR:	return "Not a directory";
    case EISDIR:	return "Is a directory";
    case EINVAL:	return "Invalid argument";
    case ENOSPC:	return "No space left on device";
    case EROFS:		return "Read-only file system";
    case ENAMETOOLONG:	return "File name too long";
    case ENOTEMPTY:	return "Directory not empty";
    }

# endif

  sprintf(str, "System error #%d", errnum);

  return str;
}
