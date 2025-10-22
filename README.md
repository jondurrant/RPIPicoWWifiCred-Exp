# RPIPicoWWifiCred-Exp
Experiments with Securing Wifi Credentials on the Pico W.
This example was built to support an episode on my Youtube channel [DrJonEA]()https://youtube.com/@drjonea).

Question is can I secure Wifi credentials on a Pico W. To immediate answer to that is NO, but I don't think that
is the full answer. Security is not a binary concept, so I don't NO is the full answer.So I wanted to see with 
the tools at my disposal how dificult I can make it to extract the Wifi Credentials from a Pico W.

## Cloaning and Build
The example here uses quite a lot of libraries as it was concieved to be a Web Service demonstration. In fact
I backed off from that but I've still included a lot of libraries. Please clone with the recurse submodule switch.

The examples are built with the Pico SDK toolchain. This should work inside VSCode using CMake extension.
Or from command line tool chain going through the process:
```
cd 'project'
mkdir build
cd build
cmake ..
make
```

## Experiments

The examples take wifi credentials  in through stdio over uart (pin 15 and 16 by default). Input is a JSON
format and utilities in the "exp/py" folder can send data to the Pico in the right format.

### exp/PlainTxt
This is an example just showing the potential problem. So it stores the credentials in plain text within an
area of Non Volatile Storage in the flash.

### exp/Crypt
This example also store the credentials in Non Volatile Storage in flash. First though it encrypts them using
AES algorithm. This then passes the problem of how to store the key. The key is a SHA256 hash of a random
string generated when cmake runs, and the Pico Unique ID. 

Though everything to hack this and get the Keys can be pulled from Flash you need to find four seperate
pieces of flash memory. You would need to obtain:
+ The IV stored in NVS Flash
+ The encrypted keys stored in NVS Flash
+ The cmake generated key, stored as a programme literal in Flash
+ The pico unique id, from pico sdk call or the usb id in bootsel mode





