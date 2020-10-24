void
signalCondition(ConditionPtr condition)
{
    ConditionHandlerPtr handler;
    int done;

    assert(!in_signalCondition);
    in_signalCondition++;

    handler = condition->handlers;
    while(handler) {
        ConditionHandlerPtr next = handler->next;
        done = handler->handler(0, handler);
        if(done) {
            if(handler == condition->handlers)
                condition->handlers = next;
            if(next)
                next->previous = handler->previous;
            if(handler->previous)
                handler->previous->next = next;
            else
                condition->handlers = next;
            free(handler);
        }
        handler = next;
    }
    in_signalCondition--;
}