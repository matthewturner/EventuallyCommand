#include <Eventually.h>
#include <EventuallyCommand.h>

EvtManager mgr;
// monitor Serial stream with an optional read
// delay (defaults to 5ms). You might need to
// set this higher depending on your baudrate
EvtCommandListener commandListener(&Serial, 5);

void setup()
{
  Serial.begin(115200);

  commandListener.when("set", (EvtCommandAction)set);
  commandListener.when("update", (EvtCommandAction)update);

  mgr.addListener(&commandListener);

  Serial.println("Setup complete. Continuing...");
}

// serial will be checked on each loop
USE_EVENTUALLY_LOOP(mgr)

// no need to specify EvtCommandAction arguments
bool set()
{
  Serial.println("Setting...");
}

// unless you are interested in the data element
bool update(EvtListener *, EvtContext *, long data)
{
  Serial.print("Updating with ");
  Serial.print(data);
  Serial.println("...");
}