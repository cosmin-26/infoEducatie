import RPi.GPIO as GPIO
from time import sleep

class Servo:
    def __init__(self, pin, freq=50, max_angle=270):
        self.pin = pin
        self.freq = freq
        self.max_angle = max_angle
        self.angle = 0  # Start at 0 degrees by default

        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(self.pin, GPIO.OUT)

        self.pwm = GPIO.PWM(self.pin, self.freq)
        self.pwm.start(0)
        sleep(0.5)

    def set_angle(self, angle):
        # Clamp angle to valid range
        angle = max(0, min(angle, self.max_angle))
        duty = 2.5 + (angle / self.max_angle) * 10
        self.pwm.ChangeDutyCycle(duty)
        sleep(0.5)
        self.pwm.ChangeDutyCycle(0)  # Avoid jitter
        self.angle = angle  # Update stored position

    def move_by(self, delta):
        self.set_angle(self.angle + delta)

    def get_angle(self):
        return self.angle

    def cleanup(self):
        self.pwm.stop()
