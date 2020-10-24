    switch(err) {
#ifdef EAI_BADFLAGS
        case EAI_BADFLAGS:
            return "Invalid value for ai_flags (EAI_BADFLAGS)";
#endif
        case EAI_NONAME:
            return "Neither nodename nor servname known (EAI_NONAME)";
#ifdef EAI_AGAIN
        case EAI_AGAIN:
            return "Temporary failure in name resolution (EAI_AGAIN)";
#endif
#ifdef EAI_FAIL
        case EAI_FAIL:
            return "Non-recoverable failure in name resolution (EAI_FAIL)";
#endif
#ifdef EAI_NODATA
#if EAI_NODATA!=EAI_NONAME
        case EAI_NODATA:
            return "No address associated with nodename (EAI_NODATA)";
#endif /* EAI_NODATA!=EAI_NONAME */
#endif /* defined EAI_NODATA */
#ifdef EAI_FAMILY
        case EAI_FAMILY:
            return "ai_family not supported (EAI_FAMILY)";
#endif
#ifdef EAI_SOCKTYPE
        case EAI_SOCKTYPE:
            return "ai_socktype not supported (EAI_SOCKTYPE)";
#endif
#ifdef EAI_SERVICE
        case EAI_SERVICE:
            return "servname is not supported for ai_socktype (EAI_SERVICE)";
#endif
#ifdef EAI_ADDRFAMILY
        case EAI_ADDRFAMILY:
            return "Address family for nodename not supported (EAI_ADDRFAMILY)";
#endif /* EAI_ADDRFAMILY */
        case EAI_MEMORY:
            return "Memory allocation failure (EAI_MEMORY)";
#ifdef EAI_SYSTEM
        case EAI_SYSTEM:
            return "System error returned in errno (EAI_SYSTEM)";
#endif /* EAI_SYSTEM */
        default:
            return "Unknown error";
    }
}
