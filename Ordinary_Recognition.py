# Untitled - By: XuChao - 周一 7月 30 2018
# Blob Detection Example
#
# This example shows off how to use the find_blobs function to find color
# blobs in the image. This example in particular looks for dark green objects.
enable_lens_corr = False
import sensor, image, time


# For color tracking to work really well you should ideally be in a very, very,
# very, controlled enviroment where the lighting is constant...

red_threshold_01 = (1, 89, 17, 81, 0, 77)
green_threshold_02 = (39, 93, -71, -28, -22, 67)
yellow_threshold_03 = (25,100,-128,127,12,119)
blue_threshold_04 =(47, 4, -128, 50, -42, -2)

color1_1 = "r"
color1_2 = "e"
color1_3 = "d"

color2_1 = "g"
color2_2 = "r"
color2_3 = "e"
color2_4 = "e"
color2_5 = "n"

color3_1 = "y"
color3_2 = "e"
color3_3 = "l"
color3_4 = "l"
color3_5 = "o"
color3_6 = "w"

color4_1 = "b"
color4_2 = "l"
color4_3 = "u"
color4_4 = "e"

shape2_1 = "c"
shape2_2 = "r"
shape2_3 = "i"
shape2_4 = "c"
shape2_5 = "l"
shape2_6 = "e"

shape1_1 = "r"
shape1_2 = "e"
shape1_3 = "c"
shape1_4 = "t"
shape1_5 = "a"
shape1_6 = "n"
shape1_7 = "g"
shape1_8 = "l"
shape1_9 = "e"

shape3_1 = "t"
shape3_2 = "r"
shape3_3 = "a"
shape3_4 = "n"
shape3_5 = "g"
shape3_6 = "l"
shape3_7 = "e"

# You may need to tweak the above settings for tracking green things...
# Select an area in the Framebuffer to copy the color settings.


#代表红色的代码
red_color_code = 1
green_color_code = 2
yellow_color_code = 4
blue_color_code = 8
from pyb import UART

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QVGA for quailtiy ,use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False)
uart = UART(3,115200) #利用串口3
#关闭白平衡。白平衡是默认开启的，在颜色识别中，需要关闭白平衡。
clock = time.clock() # Tracks FPS.
right_angle_threshold = (45, 61)
forget_ratio = 0.8
move_threshold = 5
center_x = 000
center_y = 000


def calculate_angle(line1, line2):
    # 利用四边形的角公式， 计算出直线夹角
    angle  = (180 - abs(line1.theta() - line2.theta()))
    if angle > 90:
        angle = 180 - angle
    return angle


def is_right_angle(line1, line2):
    global right_angle_threshold6666666666666
    # 判断两个直线之间的夹角是否为直角
    angle = calculate_angle(line1, line2)

    if angle >= right_angle_threshold[0] and angle <=  right_angle_threshold[1]:
        # 判断在阈值范围内
        return True
    return False

def find_verticle_lines(lines):
    line_num = len(lines)
    for i in range(line_num -1):
        for j in range(i, line_num):
            if is_right_angle(lines[i], lines[j]):
                return (lines[i], lines[j])
    return (None, None)


def calculate_intersection(line1, line2):
    # 计算两条线的交点
    a1 = line1.y2() - line1.y1()
    b1 = line1.x1() - line1.x2()
    c1 = line1.x2()*line1.y1() - line1.x1()*line1.y2()

    a2 = line2.y2() - line2.y1()
    b2 = line2.x1() - line2.x2()
    c2 = line2.x2() * line2.y1() - line2.x1()*line2.y2()

    if (a1 * b2 - a2 * b1) != 0 and (a2 * b1 - a1 * b2) != 0:
        cross_x = int((b1*c2-b2*c1)/(a1*b2-a2*b1))
        cross_y = int((c1*a2-c2*a1)/(a1*b2-a2*b1))
        return (cross_x, cross_y)
    return (-1, -1)


def draw_cross_point(cross_x, cross_y):
    img.draw_cross(cross_x, cross_y)
    img.draw_circle(cross_x, cross_y, 5)
    img.draw_circle(cross_x, cross_y, 10)
# All lines also have `x1()`, `y1()`, `x2()`, and `y2()` methods to get their end-points
# and a `line()` method to get all the above as one 4 value tuple for `draw_line()`.

old_cross_x1 = 0
old_cross_y1 = 0
old_cross_x2 = 0
old_cross_y2 = 0
old_cross_x3 = 0
old_cross_y3 = 0
'''
  扩宽roi
'''

def expand_roi(roi):
    # set for QQVGA 160*120
    extra = 5
    win_size = (160, 120)
    (x, y, width, height) = roi
    new_roi = [x-extra, y-extra, width+2*extra, height+2*extra]

    if new_roi[0] < 0:
        new_roi[0] = 0
    if new_roi[1] < 0:
        new_roi[1] = 0
    if new_roi[2] > win_size[0]:
        new_roi[2] = win_size[0]
    if new_roi[3] > win_size[1]:
        new_roi[3] = win_size[1]

    return tuple(new_roi)


while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.
    img1 = sensor.snapshot().lens_corr(1.4,1.0)
    #  pixels_threshold=100, area_threshold=100
    blobs = img.find_blobs([red_threshold_01,green_threshold_02,yellow_threshold_03,blue_threshold_04], area_threshold=150)


    if blobs:
    #如果找到了目标颜色
        print(blobs)
        for blob in blobs:
            x = blob[0]
            y = blob[1]
            width = blob[2]
            height = blob[3]
            center_x = blob[5]
            center_y = blob[6]
            color_code = blob[8]
            if color_code == red_color_code:
                img.draw_string(x, y - 10,"red", color =(0xFF,0x00,0x00))
                string = "e"
                uart.write(string)
                time.sleep(10)
                uart.write(color1_1)
                time.sleep(10)
                uart.write(color1_2)
                time.sleep(10)
                uart.write(color1_3)
                time.sleep(50)
            elif color_code == green_color_code:
                img.draw_string(x, y - 10,"green", color =(0x00, 0xFF, 0x00))
                string = "g"
                uart.write(string)
                time.sleep(10)
                uart.write(color2_1)
                time.sleep(10)
                uart.write(color2_2)
                time.sleep(10)
                uart.write(color2_3)
                time.sleep(10)
                uart.write(color2_4)
                time.sleep(10)
                uart.write(color2_5)
                time.sleep(50)
            elif color_code == yellow_color_code:
                img.draw_string(x, y - 10,"yellow", color =(255,128,0))
                string = "w"
                uart.write(string)
                time.sleep(10)
                time.sleep(10)
                uart.write(color3_1)
                time.sleep(10)
                uart.write(color3_2)
                time.sleep(10)
                uart.write(color3_3)
                time.sleep(10)
                uart.write(color3_4)
                time.sleep(10)
                uart.write(color3_5)
                time.sleep(10)
                uart.write(color3_6)
                time.sleep(50)
            elif color_code == blue_color_code:
                img.draw_string(x, y - 10,"blue", color =(0,0,139))
                string = "b"
                uart.write(string)
                time.sleep(10)
                uart.write(color4_1)
                time.sleep(10)
                uart.write(color4_2)
                time.sleep(10)
                uart.write(color4_3)
                time.sleep(10)
                uart.write(color4_4)
                time.sleep(50)
        #迭代找到的目标颜色区域
            is_circle = False
            max_circle = None
            max_radius = -1

            new_roi = expand_roi(blob.rect())

        #   img.draw_rectangle(new_roi) # rect
        #   img.draw_rectangle(blob.rect()) # rect
        #   #用矩形标记出目标颜色区域
        #   img.draw_cross(blob[5], blob[6]) # cx, cy



            for c in img.find_circles(threshold = 2000, x_margin = 20, y_margin = 20, r_margin = 10, roi=new_roi):
                is_circle = True
                # img.draw_circle(c.x(), c.y(), c.r(), color = (255, 255, 255))
                if c.r() > max_radius:
                    max_radius = c.r()
                    max_circle = c
            if is_circle:
                img.draw_string(x, y, "circle",color =(0x00,0xFF,0x00))
                # 如果有对应颜色的圆形 标记外框
                # Draw a rect around the blob.
               # img.draw_rectangle(new_roi) # rect
               # img.draw_rectangle(blob.rect()) # rect
                #用矩形标记出目标颜色区域 b
                img.draw_cross(blob[5], blob[6]) # cx, cy
                img.draw_circle(max_circle.x(), max_circle.y(), max_circle.r(), color = (0, 255, 0))
                img.draw_circle(max_circle.x(), max_circle.y(), max_circle.r() + 1, color = (0, 255, 0))

                string  = "c"
                uart.write(string)
                time.sleep(10)
                uart.write(shape2_1)
                time.sleep(10)
                uart.write(shape2_2)
                time.sleep(10)
                uart.write(shape2_3)
                time.sleep(10)
                uart.write(shape2_4)
                time.sleep(10)
                uart.write(shape2_5)
                time.sleep(10)
                uart.write(shape2_6)
                time.sleep(50)

            for r in img.find_rects(threshold = 10000):
                img.draw_rectangle(r.rect(),color = (255, 0, 0))
                img.draw_string(x, y, "rectangle", color = (0,0,0))
                img.draw_cross(blob[5],blob[6])

                string ="r"
                uart.write(string)
                time.sleep(10)
                uart.write(shape1_1)
                time.sleep(10)
                uart.write(shape1_2)
                time.sleep(10)
                uart.write(shape1_3)
                time.sleep(10)
                uart.write(shape1_4)
                time.sleep(10)
                uart.write(shape1_5)
                time.sleep(10)
                uart.write(shape1_6)
                time.sleep(10)
                uart.write(shape1_7)
                time.sleep(10)
                uart.write(shape1_8)
                time.sleep(10)
                uart.write(shape1_9)
                time.sleep(50)
            lines =  img1.find_lines(threshold = 2000, theta_margin = 40, rho_margin = 20, roi=(5, 5, 150,110))
            if len(lines) == 3:


              (cross_x, cross_y) = calculate_intersection(lines[0], lines[1])
              if cross_x != -1 and cross_y != -1:
                  if abs(cross_x - old_cross_x1) < move_threshold and abs(cross_y - old_cross_y1) < move_threshold:
                          # 小于移动阈值， 不移动
                     pass
                  else:
                       old_cross_x1 = int(old_cross_x1 * (1 - forget_ratio) + cross_x * forget_ratio)
                       old_cross_y1 = int(old_cross_y1 * (1 - forget_ratio) + cross_y * forget_ratio)
              else:
                   continue




              (cross_x, cross_y) = calculate_intersection(lines[1], lines[2])
              if cross_x != -1 and cross_y != -1:
                   if abs(cross_x - old_cross_x2) < move_threshold and abs(cross_y - old_cross_y2) < move_threshold:
                       # 小于移动阈值， 不移动
                      pass
                   else:
                        old_cross_x2 = int(old_cross_x2 * (1 - forget_ratio) + cross_x * forget_ratio)
                        old_cross_y2 = int(old_cross_y2 * (1 - forget_ratio) + cross_y * forget_ratio)
              else:
                   continue



              (cross_x, cross_y) = calculate_intersection(lines[0], lines[2])
              if cross_x != -1 and cross_y != -1:
                  if abs(cross_x - old_cross_x3) < move_threshold and abs(cross_y - old_cross_y3) < move_threshold:
                            # 小于移动阈值， 不移动
                     pass
                  else:
                       old_cross_x3 = int(old_cross_x3 * (1 - forget_ratio) + cross_x * forget_ratio)
                       old_cross_y3 = int(old_cross_y3 * (1 - forget_ratio) + cross_y * forget_ratio)
              else:
                   continue


              draw_cross_point(old_cross_x1, old_cross_y1)
              draw_cross_point(old_cross_x2, old_cross_y2)
              draw_cross_point(old_cross_x3, old_cross_y3)
              img.draw_string(x,y,"trangle",color = (50,0,255))
              string  = "t"
              uart.write(string)
              time.sleep(10)
              uart.write(shape3_1)
              time.sleep(10)
              uart.write(shape3_2)
              time.sleep(10)
              uart.write(shape3_3)
              time.sleep(10)
              uart.write(shape3_4)
              time.sleep(10)
              uart.write(shape3_5)
              time.sleep(10)
              uart.write(shape3_6)
              time.sleep(10)
              uart.write(shape3_7)
              time.sleep(50)




    # print(clock.fps()) # Note: Your OpenMV Cam runs about half as fast while
    # connected to your computer. The FPS should increase once disconnected.

    xb=str(int(center_x%1000/100))
    xs=str(int(center_x%100/10))
    xg=str(int(center_x%10/1))
    yb=str(int(center_y%1000/100))
    ys=str(int(center_y%100/10))
    yg=str(int(center_y%10/1))
    string = 'x'   #向串口发送x的中心坐标

    uart.write(string)
    time.sleep(10)
    uart.write(xb)
    time.sleep(10)
    uart.write(xs)
    time.sleep(10)
    uart.write(xg)
    time.sleep(50)


    string = 'y'  #向串口发送y的中心坐标
    uart.write(string)
    time.sleep(10)
    uart.write(yb)
    time.sleep(10)
    uart.write(ys)
    time.sleep(10)
    uart.write(yg)
    time.sleep(50)




