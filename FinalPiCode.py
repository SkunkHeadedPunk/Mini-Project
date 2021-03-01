#!/usr/bin/env python
"""Mini-Project for SEED Lab: Group 007.
Requires install of numpy, picamera, time, and opencv. 
Built for Arducam OV5647.
This program uses computer vision techniques and opencv to capture a stream of
images, and performs resize, and convert to gray operations, before detecting
Aruco Markers and the quadrant at which they appear in.
See 'README.md' for more details and instructions.
If you make changes to this file, make sure to update the shared repo;
you may use the instructions included in "raspi_git_instructions.txt"
"""

__author__ = "Jack Woolery"
__email__ = "lwoolery@mines.edu"

from picamera.array import PiRGBArray
from picamera import PiCamera
import picamera
import time
import cv2 as cv
import numpy as np
import smbus
import busio
import board
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd

bus = smbus.SMBus(1)
#Initialise I2C bus.
i2c = busio.I2C(board.SCL, board.SDA)

address = 0x04

camera = PiCamera(resolution=(960, 544))

lcd_columns = 16
lcd_rows = 2

lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_columns, lcd_rows)

lcd.clear()
# Set LCD color to blue
lcd.color = [0, 0, 100]
time.sleep(1)

def detect_marker(img):
    loc = []
    arucoDict = cv.aruco.getPredefinedDictionary(cv.aruco.DICT_4X4_100)

    corners, ids, _ = cv.aruco.detectMarkers(image=img, dictionary=arucoDict)

    if ids is not None:
        print("Marker detected")
        for tag in ids:
            cv.aruco.drawDetectedMarkers(image=img,
                                         corners=corners,
                                         ids=ids,
                                         borderColor=(0, 0, 255)
                                         )
            sum_x = 0
            sum_y = 0
            for i in range(4):
                sum_x = sum_x + corners[0][0][i, 0]
                sum_y = sum_y + corners[0][0][i, 1]
            ave_x = sum_x / 4
            ave_y = sum_y / 4
            loc.append([ave_x, ave_y])
            
        quad, img_marked = get_quadrant(loc, img)

    if ids is None:
        print("Marker not detected")
        quad = "0"
        img_marked = img

    #cv.putText(img, quad, (200, 200), cv.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255),
               #2, cv.LINE_AA)
    
        
    return img_marked, quad


def get_quadrant(loc, img):
    quad = 0
    cx = img.shape[1] / 2
    cy = img.shape[0] / 2

    loc = loc[0]
    x = loc[0]
    y = loc[1]

    if x >= cx and y < cy:
        quad = "1"

    if x < cx and y < cy:
        quad = "2"

    if x < cx and y >= cy:
        quad = "3"

    if x >= cx and y >= cy:
        quad = "4"

    cv.putText(img, quad, (0, 50), cv.FONT_HERSHEY_SIMPLEX, 2, 255, 4)
    print("Quadrant: ", quad)

    writeNumber(int(quad))
    lcd.message = "\nSet: " + str((int(quad)*90)%360)
    displayCurrPos()

    return quad, img


def writeNumber(value):
    bus.write_byte(address, value)
    return -1

def readNumber():
    number = bus.read_byte(address)
    return number

def displayCurrPos():
    i = 0
    prevPosition = 0
    while i != 3:
        currentPosition = readNumber()
        lcd.message = "Curr: " + str(currentPosition)
        if (currentPosition == prevPosition):
            i = i + 1
        prevPosition = currentPosition

if __name__ == '__main__':
    x = input("Enter the number of seconds you'd like to take photos for: ")
    # Change user input to int to set time for capture stream
    x = int(x)
    t_minus_x_seconds = time.time() + x

    camera.shutter_speed = camera.exposure_speed
    camera.exposure_mode = 'off'
    g = camera.awb_gains
    camera.awb_mode = 'off'
    camera.awb_gains = g

    with picamera.array.PiRGBArray(camera) as stream:
        while time.time() < t_minus_x_seconds:
            camera.capture(stream, format="bgr")
            img = stream.array

            gray_img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

            img_markers, quad = detect_marker(gray_img)

            cv.imshow("Markers Stream", img_markers)
            cv.waitKey(2)
            cv.destroyWindow("Markers, stream")
            # Truncate the stream to clear for next image capture
            stream.truncate(0)
            

            

    cv.destroyAllWindows()