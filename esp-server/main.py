from datetime import datetime
from flask import Flask, request, jsonify
from flask_restful import Api, Resource
from flask_cors import CORS
import sqlite3
import re

app = Flask(__name__)
CORS(app)
api = Api(app)

class Esp(Resource):
    def post(self):
        try:
            data = request.get_json()
            print("Received from ESP32:", data)

            sensor_type = self.safe_identifier(data.get("sensor_type", "unknown"))
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

if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=5081)
