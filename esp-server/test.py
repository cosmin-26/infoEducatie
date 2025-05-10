sensor ={
    "temp" : 29.2,
    "humidity": 67.0,
    "scale" : "Celsius",
}

def getSQLType(data):
    t = type(sensor.get(data)) 
    if t == int:
        return "INT"
    elif t== float:
        return "REAL"
    elif t == str:
        return "TEXT"
    elif t == bool:
        return "INTEGER"  # store as 0/1
    else:
        return "TEXT"

print([f"{key} {type(sensor.get(key))}" for key in sensor.keys()])