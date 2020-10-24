int
objectIsStale(ObjectPtr object, CacheControlPtr cache_control)
{
    int stale = 0x7FFFFFFF;
    int flags;
    int max_age, s_maxage;
    time_t date;

    if(object->flags & OBJECT_INITIAL)
        return 0;

    if(object->date >= 0)
        date = object->date;
    else if(object->age >= 0)
        date = object->age;
    else
        date = current_time.tv_sec;

    if(cache_control == NULL)
        cache_control = &no_cache_control;
    flags = object->cache_control | cache_control->flags;

    if(cache_control->max_age >= 0) {
        if(object->max_age >= 0)
            max_age = MIN(cache_control->max_age, object->max_age);
        else
            max_age = cache_control->max_age;
    } else
        max_age = object->max_age;

    if(cache_control->s_maxage >= 0) {
        if(object->s_maxage >= 0)
            s_maxage = MIN(cache_control->s_maxage, object->s_maxage);
        else
            s_maxage = cache_control->s_maxage;
    } else
        s_maxage = object->s_maxage;
    
    if(max_age >= 0)
        stale = MIN(stale, object->age + max_age);

    if(cacheIsShared && s_maxage >= 0)
        stale = MIN(stale, object->age + s_maxage);

    if(object->expires >= 0 || object->max_age >= 0)
        stale = MIN(stale, object->age + maxExpiresAge);
    else
        stale = MIN(stale, object->age + maxAge);

    /* RFC 2616 14.9.3: server-side max-age overrides expires */

    if(object->expires >= 0 && object->max_age < 0) {
        /* This protects against clock skew */
        stale = MIN(stale, object->age + object->expires - date);
    }

    if(object->expires < 0 && object->max_age < 0) {
        /* No server-side information -- heuristic expiration */
        if(object->last_modified >= 0)
            /* Again, take care of clock skew */
            stale = MIN(stale,
                        object->age +
                        (date - object->last_modified) * maxAgeFraction);
        else
            stale = MIN(stale, object->age + maxNoModifiedAge);
    }

    if(!(flags & CACHE_MUST_REVALIDATE) &&
       !(cacheIsShared && (flags & CACHE_PROXY_REVALIDATE))) {
        /* Client side can relax transparency */
        if(cache_control->min_fresh >= 0) {
            if(cache_control->max_stale >= 0)
                stale = MIN(stale - cache_control->min_fresh,
                            stale + cache_control->max_stale);
            else
                stale = stale - cache_control->min_fresh;
        } else if(cache_control->max_stale >= 0) {
            stale = stale + cache_control->max_stale;
        }
    }

    return current_time.tv_sec > stale;
}