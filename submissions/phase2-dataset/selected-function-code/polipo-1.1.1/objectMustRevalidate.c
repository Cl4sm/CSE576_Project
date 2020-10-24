int
objectMustRevalidate(ObjectPtr object, CacheControlPtr cache_control)
{
    int flags;

    if(cache_control == NULL)
        cache_control = &no_cache_control;
    if(object)
        flags = object->cache_control | cache_control->flags;
    else
        flags = cache_control->flags;
    
    if(flags & (CACHE_NO | CACHE_NO_HIDDEN | CACHE_NO_STORE))
        return 1;

    if(cacheIsShared && (flags & CACHE_PRIVATE))
        return 1;

    if(!mindlesslyCacheVary && (flags & CACHE_VARY))
        return 1;

    if(dontCacheCookies && (flags & CACHE_COOKIE))
        return 1;

    if(object)
        return objectIsStale(object, cache_control);

    return 0;
}