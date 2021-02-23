#!/usr/bin/env python
"""Exercises for SEED Lab Assignment 2.

Requires install of numpy, picamera, time, opencv, and math.
Use with file 'SEED_Assignment2_EX0'. 
Built for Arducam OV5647.

This program uses computer vision techniques to capture and manipulate images.
The program also includes printing, detection, and orientation of aruco markers
using built in opencv functions and transformation matrices.

See 'SEED_Assignment_Document.PDF' for more details and instructions.
"""

__author__ = "Jack Woolery"
__email__ = "lwoolery@mines.edu"

from picamera import PiCamera
import picamera
import time
import cv2 as cv
import numpy as np

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
            for i in range(4):
                sum_x = sum_x + corners[0][0][i, 0]
                sum_y = sum_y + corners[0][0][i, 1]
            ave_x = sum_x / 4
            ave_y = sum_y / 4
            loc.append([ave_x, ave_y])

    if ids is None:
        print("Marker not detected")
        
    return loc, img

def get_quadrant(loc, img):
    cx = img.shape[1] / 2
    cy = img.shape[0] / 2

    print("loc: ", loc)
    loc = loc[0]
    print("loc[0]: ", loc)

    return quad, output

def main():

    x = input("Enter the number of seconds you'd like to take photos for: ")
    # Change user input to int to set time for capture stream
    x = int(x)
    t_minus_x_seconds = time.time() + x

    with picamera.PiCamera() as camera:
        camera.resolution = (320, 240)
        camera.framerate = 24
        time.sleep(2)
        while time.time() < t_minus_x_seconds:
            img = np.empty((240 * 320 * 3,), dtype=np.uint8)
            camera.capture(img, 'bgr')
            img = img.reshape((240, 320, 3))

            gray_img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

            loc, img_markers = detect_marker(gray_img)

            quad, output = get_quadrant(loc, img_markers)

            cv.imshow("Markers Stream", img_markers)
            cv.waitKey(500)
            cv.destroyWindow("Markers, stream")
            # Truncate the output to clear for next image capture
            output.truncate(0)

