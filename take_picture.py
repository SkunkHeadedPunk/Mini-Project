# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2

if __name__ == '__main__':
 
   #fileName = input("File Name:")

   # initialize the camera and grab a reference to the raw camera capture
   camera = PiCamera()
   rawCapture = PiRGBArray(camera)
 
   # allow the camera to warmup
   time.sleep(0.1)
 
   # grab an image from the camera
   print("Capturing Image...")
   camera.capture(rawCapture, format="bgr")
   image = rawCapture.array
   cv2.imshow("image", image)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
