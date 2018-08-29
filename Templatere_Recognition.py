# Untitled - By: XuChao - 周四 8月 2 2018

import time, sensor, image
from image import SEARCH_EX, SEARCH_DS

from pyb import UART
# Reset sensor
sensor.reset()

# Set sensor settings
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
sensor.set_framesize(sensor.QQVGA)
# You can set windowing to reduce the search image.
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.GRAYSCALE)

# Load template.
# Template should be a small (eg. 32x32 pixels) grayscale image.
template1 = image.Image("/1.pgm")
template2 = image.Image("/2.pgm")
m = 000
n = 000
uart = UART(3,115200) #利用串口3
clock = time.clock()

# Run template matching
while (True):
    clock.tick()
    img = sensor.snapshot()

    # find_template(template, threshold, [roi, step, search])
    # ROI: The region of interest tuple (x, y, w, h).
    # Step: The loop step used (y+=step, x+=step) use a bigger step to make it faster.
    # Search is either image.SEARCH_EX for exhaustive search or image.SEARCH_DS for diamond search
    #
    # Note1: ROI has to be smaller than the image and bigger than the template.
    # Note2: In diamond search, step and ROI are both ignored.
    r = img.find_template(template1, 0.7, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
    if r:
        img.draw_rectangle(r,color=(0,0,0))
        img.draw_string(30,20,"Pentagram",color = (0,0,0))
        m=r[0]+r[2]/2.0
        m=int(m)
        n=r[1]+r[3]/2.0
        n=int(n)
        img.draw_cross(m,n)
        print(m)
        print(n)
    p = img.find_template(template2, 0.7, step=4, search=SEARCH_EX)
    if p:
        img.draw_rectangle(p,color=(255,255,255))
        img.draw_string(30,20,"Trangle",color = (0,0,0))
        a=p[0]+p[2]/2.0
        a=int(a)
        b=p[1]+p[3]/2.0
        b=int(b)
        img.draw_cross(a,b)




    #    xb=str(int(m%1000/100))
    #    xs=str(int(m%100/10))
    #    xg=str(int(m%10/1))
    #    yb=str(int(n%1000/100))
    #    ys=str(int(n%100/10))
    #    yg=str(int(n%10/1))
    #
    #    string = 'm'   #向串口发送x的中心坐标
    #    uart.write(string)
    #    time.sleep(10)
    #    uart.write(xb)
    #    time.sleep(10)
    #    uart.write(xs)
    #    time.sleep(10)
    #    uart.write(xg)
    #    time.sleep(50)
    #
    #
    #    string = 'n'  #向串口发送y的中心坐标
    #    uart.write(string)
    #    time.sleep(10)
    #    uart.write(yb)
    #    time.sleep(10)
    #    uart.write(ys)
    #    time.sleep(10)
    #    uart.write(yg)
    #    time.sleep(50)
