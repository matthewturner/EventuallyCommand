[![PlatformIO CI](https://github.com/matthewturner/Eventually-CommandListener/actions/workflows/platformio.yml/badge.svg)](https://github.com/matthewturner/Eventually-CommandListener/actions/workflows/platformio.yml)

# Eventually - Command Listener

Built on the [Eventually](https://github.com/johnnyb/Eventually) library, this provides a listener which reads commands from any Serial device.

Commands can be sent from a laptop through the serial monitor or Bluetooth-connected phone.

## Command Syntax

Commands must start with '>' and end with '!', eg:

```
>set!
// or
>update!
```

Commands can optionally include an integer data element, separated by a colon:

```
>set:123!
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
