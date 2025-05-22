import requests
import json
import time

# Change this to your Raspberry Pi's IP address
SERVER_URL = "http://127.0.0.1:5081/autobee"


# List of test payloads
test_payloads = [
    {
        "Servo1":0.01,
        "Servo2":0.01,
        "Servo3":1
    },
        {
        "Servo1":0.5,
        "Servo2":0.5,
        "Servo3":1
    }
    ,
        {
        "Servo1":1,
        "Servo2":1,
        "Servo3":1
    }
]

# Send each payload
for i, payload in enumerate(test_payloads):
    response = requests.post(SERVER_URL, json=payload)
    print("Status Code:", response.status_code)
    print("Response:", response.json())
    print()
    time.sleep(3)
