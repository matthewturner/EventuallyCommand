[![PlatformIO CI](https://github.com/matthewturner/EventuallyCommand/actions/workflows/platformio.yml/badge.svg)](https://github.com/matthewturner/EventuallyCommand/actions/workflows/platformio.yml)

# Eventually - Command

Built on the [Eventually](https://github.com/johnnyb/Eventually) library, this provides a listener which reads commands from any Serial device.

This works alongside any other listener you might already have in your sketch.

Commands can be sent from a laptop through the serial monitor or Bluetooth-connected phone.

## Command Syntax

Commands must start with **'>'** and end with **'!'**, eg:

```
>set!
// or
>update!
```

Commands can optionally include an integer data element, separated by a colon:

```
>set:123!
```

The data element will be passed into your registered EvtCommandAction callback.

## Usage

```
EvtManager mgr;
EvtCommandListener commandListener(&Serial);

void setup()
{
    Serial.begin(115200);

    // register other listeners as normal
    mgr.addListener(new EvtPinListener(...));

    // register the command and the action to trigger
    commandListener.when("update", (EvtCommandAction)update);
    commandListener.when("set", (EvtCommandAction)set);

    mgr.addListener(&commandListener);
}
```

## Installing Platform IO

Install command line tools by following the installation instructions for [Windows](https://docs.platformio.org/en/latest/core/installation.html#windows)

## Serial Monitor

Deploy code to your Arduino and run the following command in a terminal:

```powershell
 pio device monitor --eol=CRLF --echo --filter=send_on_enter
```

## Unit Testing

Run the following command after installing Platform IO:

```powershell
pio test -e native
```
