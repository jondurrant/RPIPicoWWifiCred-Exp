import os
import json
import sys
import time
import json


if len(sys.argv) == 1:
    print("Need IO Device as first parameter")
    sys.exit(1)

iodev = sys.argv[1]


state = {
    "nuke": True
}


print("Start")
with open(iodev, "w") as f:
   
    print(json.dumps(state))
    f.write(json.dumps(state) + "\n")
    f.flush()
    
    time.sleep(5)
    f.close()