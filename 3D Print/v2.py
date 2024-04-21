import numpy as np
import cv2

#masking colour range
sensitivity = 15
lower_white = np.array([0,0,255-sensitivity])
upper_white = np.array([255,sensitivity,255])

cap = cv2.VideoCapture(0)

while(True):
    #capture frame-by-frame
    ret = cap.set(3,160) #set Width 
    ret = cap.set(4, 120) #set Heigth
    #ret = cap.set(5, .0) NOT working on Logitech QuickCam: No FPS setting :(
    ret, frame = cap.read()

    #our operations on the frame come here
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv,lower_white,upper_white)
    
    

    #Display the resulting frame
    cv2.imshow('mask_frame', mask)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()