static void usage(void)
{
    apr_file_printf(errfile, "Usage:" NL
        "\thtpasswd [-cimBdpsDv] [-C cost] passwordfile username" NL
        "\thtpasswd -b[cmBdpsDv] [-C cost] passwordfile username password" NL
        NL
        "\thtpasswd -n[imBdps] [-C cost] username" NL
        "\thtpasswd -nb[mBdps] [-C cost] username password" NL
        " -c  Create a new file." NL
        " -n  Don't update file; display results on stdout." NL
        " -b  Use the password from the command line rather than prompting "
            "for it." NL
        " -i  Read password from stdin without verification (for script usage)." NL
        " -m  Force MD5 encryption of the password (default)." NL
        " -B  Force bcrypt encryption of the password (very secure)." NL
        " -C  Set the computing time used for the bcrypt algorithm" NL
        "     (higher is more secure but slower, default: %d, valid: 4 to 31)." NL
        " -d  Force CRYPT encryption of the password (8 chars max, insecure)." NL
        " -s  Force SHA encryption of the password (insecure)." NL
        " -p  Do not encrypt the password (plaintext, insecure)." NL
        " -D  Delete the specified user." NL
        " -v  Verify password for the specified user." NL
        "On other systems than Windows and NetWare the '-p' flag will "
            "probably not work." NL
        "The SHA algorithm does not use a salt and is less secure than the "
            "MD5 algorithm." NL,
        BCRYPT_DEFAULT_COST
    );
    exit(ERR_SYNTAX);
}