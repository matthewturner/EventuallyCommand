#include <Eventually.h>
#include <EvtCommandListener.h>

EvtManager mgr;
EvtCommandListener commandListener(&Serial);

void setup()
{
  Serial.begin(115200);

  commandListener.when("update", (EvtCommandAction)update);
  commandListener.when("set", (EvtCommandAction)set);

  mgr.addListener(&commandListener);

  Serial.println("Setup complete. Continuing...");
}

bool update(EvtListener *, EvtContext *, long data)
{
  Serial.print("Updating with ");
  Serial.print(data);
  Serial.println("...");
}

bool set()
{
  Serial.println("Setting...");
}

USE_EVENTUALLY_LOOP(mgr)
