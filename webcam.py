import cv2,os,subprocess

webcam = cv2.VideoCapture(0)

#key = cv2.waitKey(-1)



while True:
    ret,image = webcam.read()

    if not ret:
        print("Error")
        break
    cv2.imshow("test",image)
    grayscale = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
    k = cv2.waitKey(1)
    
    if k%256 == 27:
        #print(grayscale)
        break

    
    cv2.imwrite("alpha.pgm",grayscale,[cv2.IMWRITE_PXM_BINARY, 1])
    ret = os.system("fileDisplay.exe") 




webcam.release()
