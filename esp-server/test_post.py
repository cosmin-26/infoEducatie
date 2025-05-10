import requests
import json

# Change this to your Raspberry Pi's IP address
SERVER_URL = "http://127.0.0.1:5081/esp-data"


# List of test payloads
test_payloads = [
    {
        "sensor_type": "co2",
        "data": {
            "ppm": 415,
            "alert": False
        },
        "date": "2025-04-30T13:55:00"
    },
    {
        "sensor_type": "light",
        "data": {
            "lux": 300,
            "source": "window",
            "status": "normal"
        },
        "date": "2025-04-30T14:00:00"
    },
    {
        "sensor_type": "motion",
        "data": {
            "detected": True,
            "zone": "garage",
            "sensitivity": 0.85
        },
        "date": "2025-04-30T14:05:00"
    }
]

# Send each payload
for i, payload in enumerate(test_payloads):
    print(f"--- Sending payload {i+1} ({payload['sensor_type']}) ---")
    response = requests.post(SERVER_URL, json=payload)
    print("Status Code:", response.status_code)
    print("Response:", response.json())
    print()
