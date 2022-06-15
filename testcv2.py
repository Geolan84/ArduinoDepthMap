from threading import Thread
import sys 
import serial
import time
import cv2
import numpy as np

global left, up, right, centr
left = 0
up = 0
right = 0
centr = 0
global flag
flag = True

ard = serial.Serial('COM3',9600, timeout = 5)
cap = cv2.VideoCapture(1)
#Параметры текста и графических элементов.
font = cv2.FONT_HERSHEY_SIMPLEX
fontScale = 1 
color = (255, 0, 0)
org = (600, 450)  
thickness = 2

#Функция считывания данных.
def func():
    while (flag):
        #Обрезаем два последних символа.
        msg = ard.readline()[:-2]
        length = len(msg)
        if (msg==b'click!'):
            print("click!")
        elif (length>2)&(length!=6):
            print(msg)
            array1 = msg.split()
            global left, centr, right, up
            left = array1[0].decode('utf-8')
            up = array1[1].decode('utf-8')
            right = array1[2].decode('utf-8')
            centr = array1[3].decode('utf-8')
    return
#Функция камеры и рисования.
def funcCam():
    while (True):
        ret, frame = cap.read()

        
        #фон для показаний центра
        cv2.rectangle(frame, (270,300), (350, 350), (0, 255, 0), -1)
        #измерения центра
        if (int(centr)<=80):
            cv2.putText(frame, str(centr), (275, 337), font, fontScale, color, thickness, cv2.LINE_AA)
        else:
            cv2.rectangle(frame, (270,300), (350, 350), (0, 0, 255), -1)
        #рамка центрального луча
        cv2.rectangle(frame, (350,300), (400, 350), (0, 0, 255), thickness)



        #верхняя стрелка
        triangle_cnt_up = np.array( [(0,0), (640,0), (330, 50)] )
        cv2.drawContours(frame, [triangle_cnt_up], 0, color, 2)
        #фон верхнего
        cv2.rectangle(frame, (300, 55), (365, 85), (0, 255, 0), -1)
        #измерения верхнего
        cv2.putText(frame, str(up), (300, 80), font,  
                   fontScale, color, thickness, cv2.LINE_AA)



        #левая стрелка
        triangle_cnt_left = np.array( [(0,0), (0,480), (50, 240)] )
        cv2.drawContours(frame, [triangle_cnt_left], 0, color, 2)
        #фон левого
        cv2.rectangle(frame, (55, 255), (120, 223), (0, 255, 0), -1)
        #измерения левого
        cv2.putText(frame, str(left), (55, 250), font,  
                   fontScale, color, thickness, cv2.LINE_AA)



        #правая стрелка
        triangle_cnt_right = np.array( [(640,0), (640,480), (590, 240)] )
        cv2.drawContours(frame, [triangle_cnt_right], 0, color, 2)
        #фон правого
        cv2.rectangle(frame, (585, 255), (520, 223), (0, 255, 0), -1)
        #измерения правого
        cv2.putText(frame, str(right), (520, 250), font,  
                   fontScale, color, thickness, cv2.LINE_AA)
        

         
        cv2.imshow('frame',frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            global flag
            flag = False
            break
    cap.release()
    cv2.destroyAllWindows()
    return

thread2 = Thread(target=func)
thread1 = Thread(target=funcCam)

thread1.start()
thread2.start()
thread1.join()
thread2.join()
