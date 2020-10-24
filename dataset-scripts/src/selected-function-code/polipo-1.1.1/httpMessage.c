char *
httpMessage(int code)
{
    switch(code) {
    case 200:
        return "Okay";
    case 206:
        return "Partial content";
    case 300:
        return "Multiple choices";
    case 301:
        return "Moved permanently";
    case 302:
        return "Found";
    case 303:
        return "See other";
    case 304:
        return "Not changed";
    case 307:
        return "Temporary redirect";
    case 401:
        return "Authentication Required";
    case 403:
        return "Forbidden";
    case 404:
        return "Not found";
    case 405:
        return "Method not allowed";
    case 407:
        return "Proxy authentication required";
    default:
        return "Unknown error code";
    }
}