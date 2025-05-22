from datetime import datetime
from flask import Flask, request, jsonify,Response
from flask_restful import Api, Resource
from flask_cors import CORS
import pandas as pd
import matplotlib
import sqlite3
import re
import atexit
from gpiozero import Servo
from gpiozero.pins.pigpio import PiGPIOFactory
import cv2
import apriltag
import numpy as np

app = Flask(__name__)


factory = PiGPIOFactory()

app = Flask(__name__)
CORS(app)
api = Api(app)

factory = PiGPIOFactory()
servo1 = Servo(17,min_pulse_width=500/1_000_000,max_pulse_width=2500/1_000_000,pin_factory=factory,frame_width=0.02)
servo2 = Servo(18,min_pulse_width=500/1_000_000,max_pulse_width=2500/1_000_000,pin_factory=factory,frame_width=0.02)
servo3 = Servo(27,min_pulse_width=500/1_000_000,max_pulse_width=2500/1_000_000,pin_factory=factory,frame_width=0.02)

class AutoBee(Resource):
    def post(self):
        try:
            data = request.get_json()
            print(f"AutoBee data:{data}")
            if pos:=data.get("Servo1"):
                servo1.value = pos * 2 - 1
            if pos:=data.get("Servo2"):
                servo2.value = pos * 2 - 1
            if pos:=data.get("Servo3"):
                servo3.value = pos * 2 - 1
            return jsonify({"Status":"Recevied","Data":data})
            
        except Exception as e:
            return jsonify({"error": str(e)})



class Esp(Resource):
    def post(self):
        try:
            data = request.get_json()
            print("Received from ESP32:", data)

            sensor_type = f"{self.safe_identifier(data.get('sensor_type', 'unknown'))}:{data.get('room')}" 
            sensor_data_raw = data.get("data", {})
            sensor_data = self.flatten_data(sensor_data_raw)

            if "date" in data:
                sensor_data["date"] = data["date"]
            else:
                now = datetime.now()
                sensorDate = now.strftime("%d/%m/%Y %H:%M:%S")
                sensor_data["date"] = sensorDate
            


            conn = sqlite3.connect("esp.db")
            cursor = conn.cursor()

            # Check if table exists
            cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name=?", (sensor_type,))
            if not cursor.fetchone():
                # Create table with dynamic fields
                columns_sql = ', '.join(
                    f'"{self.safe_identifier(key)}" {self.get_sqlite_type(value)}'
                    for key, value in sensor_data.items()
                )
                cursor.execute(f'''
                    CREATE TABLE "{sensor_type}" (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        {columns_sql}
                    )
                ''')

            # Prepare data for insertion
            columns = [self.safe_identifier(k) for k in sensor_data.keys()]
            keys_sql = ', '.join(f'"{k}"' for k in columns)
            placeholders = ', '.join(['?'] * len(columns))
            values = list(sensor_data.values())

            print(keys_sql)
            print(values)

            cursor.execute(f'''
                INSERT INTO "{sensor_type}" ({keys_sql})
                VALUES ({placeholders})
            ''', values)

            conn.commit()
            conn.close()

            return jsonify({"status": "received", "data": sensor_data})
        except Exception as e:
            return jsonify({"error": str(e)})

    def get(self):
        return {"message": "ESP endpoint ready!"}

    def get_sqlite_type(self, value):
        t = type(value)
        if t == int:
            return "INTEGER"
        elif t == float:
            return "REAL"
        elif t == str:
            return "TEXT"
        elif t == bool:
            return "INTEGER"  # store as 0/1
        else:
            return "TEXT"

    def safe_identifier(self, name):
        return re.sub(r'\W+', '_', name)
    
    def flatten_data(self, data, parent_key=''):
        items = {}
        for k, v in data.items():
            new_key = f"{parent_key}_{k}" if parent_key else k
            if isinstance(v, dict):
                items.update(self.flatten_data(v, new_key))
            else:
                items[new_key] = v
        return items

# Register endpoint
api.add_resource(Esp, "/esp-data")
api.add_resource(AutoBee, "/autobee")

def generate():
    cap = cv2.VideoCapture("http://192.168.0.104/stream")
    detector = apriltag.Detector()
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        frame = cv2.rotate(frame, cv2.ROTATE_180)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        results = detector.detect(gray)

        frame_height, frame_width = frame.shape[:2]
        center_x = frame_width // 2

        if results:
            # Assume first detected AprilTag for guidance
            r = results[0]
            tag_center = r.center
            tag_x = int(tag_center[0])

            # Draw the tag outline and center dot
            corners = r.corners
            corners = np.array(corners, dtype=np.int32)
            cv2.polylines(frame, [corners], True, (0, 255, 0), 2)
            cv2.circle(frame, (tag_x, int(tag_center[1])), 5, (0, 0, 255), -1)

            # Calculate horizontal difference
            diff_x = tag_x - center_x

            # Set threshold for "centered"
            threshold = 40

            # Show instruction on frame
            if abs(diff_x) < threshold:
                direction = "Centered"
                color = (0, 255, 0)
            elif diff_x > 0:
                direction = "Move Camera Right"
                color = (0, 0, 255)
            else:
                direction = "Move Camera Left"
                color = (0, 0, 255)


            # Draw text on frame
            cv2.putText(frame, direction, (30, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2, cv2.LINE_AA)
        else:
            cv2.putText(frame, "No AprilTag Detected", (30, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2, cv2.LINE_AA)

        # Encode frame to JPEG
        _, buffer = cv2.imencode('.jpg', frame)
        frame_bytes = buffer.tobytes()

        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame_bytes + b'\r\n')

@app.route('/video_feed')
def video_feed():
    return Response(generate(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/stream')
def index():
    return "<h1>AprilTag Stream</h1><img src='/video_feed'>"


if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=5081)
