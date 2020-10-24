ConditionHandlerPtr
conditionWait(ConditionPtr condition,
              int (*handler)(int, ConditionHandlerPtr),
              int dsize, void *data)
{
    ConditionHandlerPtr chandler;

    assert(!in_signalCondition);

    chandler = malloc(sizeof(ConditionHandlerRec) - 1 + dsize);
    if(!chandler)
        return NULL;

    chandler->condition = condition;
    chandler->handler = handler;
    /* Let the compiler optimise the common case */
    if(dsize == sizeof(void*))
        memcpy(chandler->data, data, sizeof(void*));
    else if(dsize > 0)
        memcpy(chandler->data, data, dsize);

    if(condition->handlers)
        condition->handlers->previous = chandler;
    chandler->next = condition->handlers;
    chandler->previous = NULL;
    condition->handlers = chandler;
    return chandler;
}