from flask import Flask, Response
import cv2
import apriltag
import numpy as np

app = Flask(__name__)
cap = cv2.VideoCapture(0)
detector = apriltag.Detector()

def generate():
    while True:
        ret, frame = cap.read()
        if not ret:
            break

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

@app.route('/')
def index():
    return "<h1>AprilTag Stream</h1><img src='/video_feed'>"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)
