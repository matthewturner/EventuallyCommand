#ifndef EVT_COMMAND_ACTION_H
#define EVT_COMMAND_ACTION_H

class IEvtContext;
class IEvtListener;

typedef bool (*EvtCommandAction)(IEvtListener *, IEvtContext *, long data);

#endif