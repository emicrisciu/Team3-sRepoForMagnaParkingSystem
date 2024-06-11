import cv2
import numpy as np
import imutils
import pytesseract
from picamera2 import Picamera2, Preview
from libcamera import controls
import time
from license_plate import RoLicensePlate

with open("/home/team3/Desktop/proiect/fisier.txt", "w") as f:
	f.write("FALSE")
	f.write("\n")

picam2=Picamera2()
camera_config=picam2.create_preview_configuration()
picam2.configure(camera_config)
picam2.start_preview(Preview.QTGL)
picam2.start()
time.sleep(3)
picam2.capture_file("/home/team3/Desktop/proiect/poza_masina.jpg")
#picam2.start(show_preview=True)

#picam2.set_controls({"AfMode": controls.AfModeEnum.Continuous})
image = cv2.imread('/home/team3/Desktop/proiect/poza_masina.jpg')
image= cv2.resize(image,(620,480) )




gray= cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)


gray= cv2.bilateralFilter(gray, 11, 17, 17)

#gray=cv2.equalizeHist(gray)
#gray=cv2.equalizeHist(gray)

#cv2.imshow('colored', image)
#cv2.imshow('gray_scaled_blurred', gray)
edged= cv2.Canny(gray,30,200)

#cv2.imshow('edged', edged)
cnts=cv2.findContours(edged.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
cnts=imutils.grab_contours(cnts)
cnts=sorted(cnts, key=cv2.contourArea, reverse=True)[:10]
screenCnt=None

for c in cnts:
# approximate the contour
	peri = cv2.arcLength(c, True)
	approx = cv2.approxPolyDP(c, 0.015 * peri, True)
# if our approximated contour has four points, then
# we can assume that we have found our screen
	if len(approx) == 4:
		screenCnt = approx
		break
		
		
if screenCnt is None:

	detected = 0

	# print("No contour detected")

else:

	detected = 1


if detected == 1:
	cv2.drawContours(image, [screenCnt], -1, (0, 255, 0),20)

mask = np.zeros(gray.shape,np.uint8)
new_image = cv2.drawContours(mask,[screenCnt],0,255,-1,)
new_image = cv2.bitwise_and(image,image,mask=mask)

(x, y) = np.where(mask == 255)
(topx, topy) = (np.min(x), np.min(y))
(bottomx, bottomy) = (np.max(x), np.max(y))
#print(bottomx)
Cropped = gray[topx:bottomx+1, topy:bottomy-1]

#blur= cv2.GaussianBlur(Cropped, (3,3), 0)
thresh=cv2.threshold(Cropped, 0 ,255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)[1]
kernel=cv2.getStructuringElement(cv2.MORPH_RECT, (3,3))
opening=cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations=1)
invert=255-opening
cv2.imshow('gray_scaled', invert)


text = pytesseract.image_to_string(invert,lang='eng',config='--psm 11 --oem 3 -c tessedit_char_whitelist=0123456789QWERTYUIOPASDFGHJKLZXCVBNM')


print("Detected Number is:",str(text))
license_detected=RoLicensePlate(str(text))
print(license_detected)

#with open("/home/team3/Desktop/proiect/fisier.txt", "w") as f:
#	f.write(str(license_detected))
	
#import subprocess
#subprocess.call(["gcc", "/home/team3/Desktop/proiect/baza_de_date.c"])

if(license_detected.validate_license_plate()==True):
	rez="TRUE"
else:
	rez="FALSE"
	
with open("/home/team3/Desktop/proiect/fisier.txt", "w") as f:
	f.write(rez)
	f.write("\n")
	f.write(str(license_detected))
	
	
import subprocess
if rez=="TRUE":
	subprocess.call(["gcc", "/home/team3/Desktop/proiect/baza_de_date.c"])
	
	
	
		

#cv2.imshow('poza contur', image)
#cv2.imshow('poza contur license', new_image)
#cv2.imshow('poza cropped', Cropped)
#cv2.waitKey()
#cv2.destroyAllWindows()


#print("alooo")
#print(license_detected.validate_license_plate())


