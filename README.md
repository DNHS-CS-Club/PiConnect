# Internet-Sign

## What does this do?
This is a simple C script that utilizes the Raspberry Pi GPIO to drive an open sign which indicates whether the internet is up or down.
When the Pi can successfully ping Google's DNS, it outputs a 3.3v current to a relay (seen below), allowing wall power to turn on the sign.
When the internet is down, the signal is cut and the sign turns off.
