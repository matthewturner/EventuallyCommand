#ifndef EVT_COMMAND_ACTION_H
#define EVT_COMMAND_ACTION_H

class EvtContext;
class EvtListener;

typedef bool (*EvtCommandAction)(EvtListener *, EvtContext *, long data);

#endif