#include <Eventually.h>
#include "EvtCommandListener.h"

EvtManager mgr;
EvtCommandListener commandListener(&Serial);

void setup()
{
  Serial.begin(115200);

  commandListener.when("update", (EvtAction)update);
  commandListener.when("set", (EvtAction)set);

  mgr.addListener(&commandListener);

  Serial.println("Setup complete. Continuing...");
}

bool update()
{
  Serial.println("Updating...");
}

bool set()
{
  Serial.println("Setting...");
}

USE_EVENTUALLY_LOOP(mgr)