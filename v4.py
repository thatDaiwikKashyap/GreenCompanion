import numpy as np 
import cv2
#motor
from gpiozero import Motor
#motor1 = Motor(4, 14) #pins 4 n 14
  
# Capturing video through webcam 
webcam = cv2.VideoCapture(0) 

  
# Start a while loop 
while(1): 
      
    # Reading the video from the 
    # webcam in image frames 
    _, imageFrame = webcam.read() 
    
    if np.mean(imageFrame) == 0:
        print("Frame is all black")
        continue
  
    # Convert the imageFrame in  
    # BGR(RGB color space) to  
    # HSV(hue-saturation-value) 
    # color space 
    hsvFrame = cv2.cvtColor(imageFrame, cv2.COLOR_BGR2HSV) 
  
    # Set range for white color and  
    # define mask 
    sensitivity = 15
    lower_white = np.array([0,0,255-sensitivity])
    upper_white = np.array([255,sensitivity,255])
    white_mask = cv2.inRange(hsvFrame, lower_white, upper_white)
  
    
    # Morphological Transform, Dilation 
    # for each color and bitwise_and operator 
    # between imageFrame and mask determines 
    # to detect only that particular color 
    kernel = np.ones((5, 5), "uint8") 
      
    # For white color 
    white_mask = cv2.dilate(white_mask, kernel) 
    res_white = cv2.bitwise_and(imageFrame, imageFrame,mask = white_mask)
      
    
    # Creating contour to track white color 
    contours, hierarchy = cv2.findContours(white_mask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 
      
    for pic, contour in enumerate(contours): 
        area = cv2.contourArea(contour) 
        if(area > 300): 
            x, y, w, h = cv2.boundingRect(contour) 
            imageFrame = cv2.rectangle(imageFrame, (x, y),  
                                       (x + w, y + h),  
                                       (0, 0, 255), 2) 
              
            cv2.putText(imageFrame, "White Colour", (x, y), 
                        cv2.FONT_HERSHEY_SIMPLEX, 1.0, 
                        (0, 0, 255))     
  
   
    # Program Termination 
    cv2.imshow("Multiple Color Detection in Real-TIme", imageFrame) 
    if cv2.waitKey(10) & 0xFF == ord('q'): 
        webcam.release() 
        cv2.destroyAllWindows() 
        break