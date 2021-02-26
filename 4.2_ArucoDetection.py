#!/usr/bin/env python
"""Mini-Project for SEED Lab: Group 007.

Requires install of numpy, picamera, time, and opencv. 
Built for Arducam OV5647.

This program uses computer vision techniques and opencv to capture a stream of
images, and performs resize, and convert to gray operations, before detecting
Aruco Markers and the quadrant at which they appear in.

See 'README.md' for more details and instructions.
If you make changes to this file, make sure to update the shared repo;
you may use the instructions included in "raspi_git_instructions.txt".
"""

__author__ = "Jack Woolery"
__email__ = "lwoolery@mines.edu"

from picamera.array import PiRGBArray
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

    cv.putText(img, quad, (0, 25), cv.FONT_HERSHEY_SIMPLEX, 1, 255, 2)
    print("Quadrant: ", quad)

    return quad, img

if __name__ == '__main__':
    camera = PiCamera()
    x = input("Enter the number of seconds you'd like to take photos for: ")
    # Change user input to int to set time for capture stream
    x = int(x)
    t_minus_x_seconds = time.time() + x

    with picamera.array.PiRGBArray(camera) as output:
        while time.time() < t_minus_x_seconds:
            camera.capture(output, format="bgr")
            img = output.array

            gray_img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

            scale = 0.5
            resize_img = cv.resize(gray_img, None, fx=scale, fy=scale,
                                   interpolation = cv.INTER_CUBIC)

            img_markers, quad = detect_marker(resize_img)

            

            cv.imshow("Markers Stream", img_markers)
            cv.waitKey(100)
            cv.destroyWindow("Markers, stream")
            # Truncate the output to clear for next image capture
            output.truncate(0)

    cv.destroyAllWindows()

