#pragma once

class EvtContext;
class EvtListener;

typedef bool (*EvtCommandAction)(EvtListener *, EvtContext *, long data);