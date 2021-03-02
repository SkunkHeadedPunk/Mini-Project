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

bus = smbus.SMBus(1)
# Initialize I2C bus
i2c = busio.I2C(board.SCL, board.SDA)

address = 0x04

# Initialize camera with reduced resolution
camera = PiCamera(resolution=(960, 544))


def detect_marker(img):
    # Get the chosen Aruco dictionary
    arucoDict = cv.aruco.getPredefinedDictionary(cv.aruco.DICT_4X4_100)
    # Get Aruco marker ids and corner locations from image
    corners, ids, _ = cv.aruco.detectMarkers(image=img, dictionary=arucoDict)
    # Initialize list for marker center location
    loc = []
    if ids is not None:
        # If marker is detected...
        print("Marker detected")
        # Code for support of more than one tag for potential future use
        for tag in ids:
            # Label Aruco marker on image
            cv.aruco.drawDetectedMarkers(image=img,
                                         corners=corners,
                                         ids=ids,
                                         borderColor=(0, 0, 255)
                                         )
            # Find the average x and y values of the corners to notate
            # the center of the marker
            sum_x = 0
            sum_y = 0
            for i in range(4):
                sum_x = sum_x + corners[0][0][i, 0]
                sum_y = sum_y + corners[0][0][i, 1]
            ave_x = sum_x / 4
            ave_y = sum_y / 4
            # Add marker center to loc list
            loc.append([ave_x, ave_y])
           
        quad, img_marked = get_quadrant(loc, img)

    if ids is None:
        # If no markers detected, return 0
        print("Marker not detected")
        quad = "0"
        img_marked = img    
        
    return img_marked, quad


def get_quadrant(loc, img):
    # Initialize quad to 0
    quad = "0"
    # Get center x and y pixel values
    cx = img.shape[1] / 2
    cy = img.shape[0] / 2
    # Extract marker center x and y values from list
    loc = loc[0]
    x = loc[0]
    y = loc[1]
    
    # Use if statements to determine quadrant of marker center
    if x >= cx and y < cy:
        quad = "1"

    if x < cx and y < cy:
        quad = "2"

    if x < cx and y >= cy:
        quad = "3"

    if x >= cx and y >= cy:
        quad = "4"

    # Print the quadrant on the image, and in the shell line
    cv.putText(img, quad, (0, 50), cv.FONT_HERSHEY_SIMPLEX, 2, 255, 4)
    print("Quadrant: ", quad)

    return quad, img


def writeNumber(value):
    bus.write_byte(address, value)
    return -1

def readNumber():
    number = bus.read_byte_data(address)
    return number



if __name__ == '__main__':
    x = input("Enter the number of seconds you'd like to take photos for: ")
    # Change user input to int to set time for capture stream
    x = int(x)
    t_minus_x_seconds = time.time() + x

    # Set camera settings and calibration
    camera.shutter_speed = camera.exposure_speed
    camera.exposure_mode = 'off'
    g = camera.awb_gains
    camera.awb_mode = 'off'
    camera.awb_gains = g

    # Set up array capture with while loop for given time
    with picamera.array.PiRGBArray(camera) as stream:
        while time.time() < t_minus_x_seconds:
            camera.capture(stream, format="bgr")
            img = stream.array
            # Convert image to gray for Aruco detection
            gray_img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
            # Function to detect Aruco marker and quadrant
            img_markers, quad = detect_marker(gray_img)

            cv.imshow("Markers Stream", img_markers)
            cv.waitKey(2)
            cv.destroyWindow("Markers, stream")
            # Truncate the stream to clear for next image capture
            stream.truncate(0)

    cv.destroyAllWindows()

