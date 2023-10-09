import sys

import pyautogui
import time
from tkinter import messagebox
import tkinter as tk
import Image
from  tkinter import font
import os
import win32gui
import win32con
import win32api
import subprocess#用来管理开启的程序的
import psutil
import threading
import signal
import xml.etree.ElementTree as ET


#弹框提示信息
def showMessagebox(top):
    if top.winfo_ismapped():
       pass
    else:
        top.mainloop()

#打开烧录程序
def start(exe_path):
    process = subprocess.Popen([exe_path])
    return process

def teststart(path):
    os.system(path)

#移动到指定的区域
def moveEXE():
    # hand = win32gui.FindWindow('AvbKeyWriter.exe',None)
    # win32gui.SetWindowPos(hand,win32gui.HWND_TOP,500,500,0,0,win32con.SWP_NOSIZE)
    # 枚举所有顶级窗口，找到你的程序窗口
    hwnd = win32gui.FindWindow(None, "Notepad")  # 将"程序窗口标题"替换为你的程序窗口的标题
    win32gui.SetWindowPos(hwnd, win32con.HWND_TOPMOST, 500, 500, 0, 0, win32con.SWP_NOMOVE | win32con.SWP_NOSIZE)


#鼠标自动选择按键和路径
def chooseBtnAndPath(path,s_img,b_img):
    #定位到box位置点击
    if b_img == 0:#此时是前面的按键没有点击
        center = pyautogui.locateOnScreen(s_img)
        time.sleep(0.3)
        pyautogui.click(center)
        time.sleep(0.5)
        # 定位到...的位置
        pos = center.left + 420
        pyautogui.click(pos)
    else:#此时是后面的路径没有选择
        center = pyautogui.locateOnScreen(b_img, s_img)
        time.sleep(0.1)
        pyautogui.click(center)
        #如果是1代表选择第一个文件
        pos = center.left + 50
        pyautogui.click(pos)
    time.sleep(1)
    pyautogui.typewrite(path)
    time.sleep(0.5)
    pyautogui.press('space')
    pyautogui.press('enter')

#鼠标自动选择路径
def choosePath(path,img):
    image = Image.open(img)
    center = pyautogui.locateOnScreen(image)
    #移动到...的位置
    pos = center.left + 50
    pyautogui.moveTo(pos)
    pyautogui.click(pos)
    time.sleep(0.5)
    pyautogui.typewrite(path)
    pyautogui.press('space')
    pyautogui.press('enter')

#选择固件程序上的固件操作
def chooseFirPath(firmware_path,image1,image2):
    center = pyautogui.locateOnScreen(image1)
    time.sleep(0.3)
    pyautogui.click(center)
    time.sleep(0.5)
    center = pyautogui.locateOnScreen(image2)
    time.sleep(0.3)
    pyautogui.click(center)
    time.sleep(1)
    pyautogui.typewrite(firmware_path)
    time.sleep(0.5)
    pyautogui.press('space')
    pyautogui.press('enter')

#初始化操作，固件的程序上选择路径
def firExeInit(firmware_path):
    image1 = Image.open('up1.jpg')
    image2 = Image.open('up2.jpg')
    chooseFirPath(firmware_path,image1,image2)



#初始化操作，烧录的程序上选择路径
def burnExeInit(at_perm_path,at_rsa_perm_path):
    image = Image.open('box.jpg')
    chooseBtnAndPath(at_perm_path,image,0)
    time.sleep(0.5)
    chooseBtnAndPath(at_rsa_perm_path, image,0)


#检查操作，监测屏幕，确保路径都被选择
def check(pic):
    image = Image.open(pic)
    location = pyautogui.locateOnScreen(image)
    return location

#重启程序的操作
def killAndRestart(process,exe_path,at_perm_path,at_rsa_perm_path):
    process.kill()
    time.sleep(1)
    # 重启
    pro = start(exe_path)
    time.sleep(1)
    burnExeInit(at_perm_path,at_rsa_perm_path)
    return pro



#终止程序
def killEXE():
    #process.kill()
    sys.exit()

#获取烧录文件的路径,解析xml文件
def getPath():
    #解析xml文件
    tree = ET.parse("path.xml")
    #获取根元素
    root = tree.getroot()
    tool_exe_path = firmware_path = exe_path = at_perm_path = at_rsa_perm_path = None
    #遍历
    for child in root:
        if child.tag == 'tool_exe_path':#固件程序的运行路径
            tool_exe_path = child.text
        if child.tag == 'firmware_path':#固件的路径
            firmware_path = child.text
        if child.tag == 'exe_path':#AvbKeyWriter程序的运行路径
            exe_path = child.text
        if child.tag == 'at_perm_path':#烧录文件1路径
            at_perm_path = child.text
        if child.tag == 'at_rsa_perm_path':#烧录文件2路径
            at_rsa_perm_path = child.text
    return tool_exe_path, firmware_path, exe_path, at_perm_path, at_rsa_perm_path

#主函数
def main():
    #从xml文件获取路径名
    tool_exe_path, firmware_path, exe_path, at_perm_path, at_rsa_perm_path = getPath()
    #打开固件软件
    fir_exe = start(tool_exe_path)
    #teststart(tool_exe_path)
    time.sleep(1)
    #窗口选择操作
    firExeInit(firmware_path)
    time.sleep(0.5)

    #打开烧录程序
    process = start(exe_path)
    time.sleep(1)
    #窗口选择操作
    burnExeInit(at_perm_path,at_rsa_perm_path)

    while True:
        # 如果固件程序存在
        if fir_exe.poll() is None:
            pass
        else:  # 程序不存在就重启
            # 打开固件软件
            fir_exe = start(tool_exe_path)
            time.sleep(0.5)
            # 窗口选择操作
            firExeInit(firmware_path)

        # 如果烧录程序存在
        if process.poll() is None:
            loc = check("box.jpg")
            #loc1 = check("box1.jpg")
            loc2 = check("box2.jpg")
            loc3 = check("box3.jpg")

            #loc4 = check("box4.jpg")
            loc5 = check("box5.jpg")
            loc6 = check("box6.jpg")

            #img_box = Image.open('box.jpg')
            #img1 = Image.open("box1.jpg")
            #img4 = Image.open("box4.jpg")

            # 1.第一个烧录文件
            if loc:
                # 重启程序
                process = killAndRestart(process,exe_path,at_perm_path,at_rsa_perm_path)
                # chooseBtnAndPath(r'D:\LoadCode\atx_permanent_attributes.bin', img1, img_box)
            elif loc2:
                process = killAndRestart(process,exe_path,at_perm_path,at_rsa_perm_path)
                #choosePath(r'D:\LoadCode\atx_permanent_attributes.bin', r"box2.jpg")
            elif loc3:
                process = killAndRestart(process,exe_path,at_perm_path,at_rsa_perm_path)
               #choosePath(r'D:\LoadCode\atx_permanent_attributes.bin', r"box3.jpg")
            # 2.第二个烧录文件D:\LoadCode\atx_permanent_attributes.bin
            #elif loc:
                #process = killAndRestart(process,exe_path,at_perm_path,at_rsa_perm_path)
                #chooseBtnAndPath(r'D:\LoadCode\permanent_attributes_cer.bin', img4, img_box)
            elif loc5:
                process = killAndRestart(process,exe_path,at_perm_path,at_rsa_perm_path)
                #choosePath(r'D:\LoadCode\permanent_attributes_cer.bin', r"box5.jpg")
            elif loc6:
                process = killAndRestart(process,exe_path,at_perm_path,at_rsa_perm_path)
                #choosePath(r'D:\LoadCode\permanent_attributes_cer.bin', r"box6.jpg")'''
            else:
                continue
        else:  # 如果程序被关掉，就重启
            process = start(exe_path)
            time.sleep(0.5)
            burnExeInit(at_perm_path,at_rsa_perm_path)

        time.sleep(1)

def py_start():
    python_thread = threading.Thread(target=main)
    python_thread.start()
    python_thread.join()


    '''if python_thread.is_alive():#线程存在
        pass
    else:#线程不存在退出
        sys.exit()'''

py_start()
'''
while True:
    image = Image.open('box.jpg')
    image1 = Image.open('box1.jpg')
    image2 = Image.open('box2.jpg')
    image3 = Image.open('box3.jpg')

    image4 = Image.open('box4.jpg')
    image5 = Image.open('box5.jpg')
    image6 = Image.open('box6.jpg')
    image7 = Image.open('box7.jpg')

    #查找烧录文件是否选择
    location = pyautogui.locateOnScreen(image)
    location1 = pyautogui.locateOnScreen(image1)
    location2 = pyautogui.locateOnScreen(image2)
    location3 = pyautogui.locateOnScreen(image3)

    location4 = pyautogui.locateOnScreen(image4)
    location5 = pyautogui.locateOnScreen(image5)
    location6 = pyautogui.locateOnScreen(image6)
    location7 = pyautogui.locateOnScreen(image7)

    top = initWin()
    #time.sleep(0.5)
    if location:# | location2:#未选择就弹框
        print("xxx")
        showMessagebox(top)
    elif location4:
        showMessagebox(top)
    elif location5:
        showMessagebox(top)
    elif location6:
        showMessagebox(top)
    elif location7:
        showMessagebox(top)
    else:#
        if top.winfo_ismapped():
            print("弹框存在")
            destoryWin(top)
        print("弹框不存在")
        destoryWin(top)
        continue
    time.sleep(1)
'''


'''
#检测程序是否存在，存在true，不存在false
def check_if_process_running(process_name):
    for proc in psutil.process_iter(['name']):
        try:
            #if process_name.lower() in proc.info['name'].lower():
            if proc.info['name'] == process_name:
                print(process_name)
                return True
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass
    return False

#关闭应用程序
def kill_process(process_name):
    # 查找进程
    for proc in psutil.process_iter(['name']):
        try:
            # 检查进程名称是否匹配
            if proc.info['name'] == process_name:
                proc.kill()  # 如果匹配，关闭进程
                print(f"Process '{process_name}' has been terminated.")
                return
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            print(f"Error in terminating process: {process_name}")
    print(f"No process found with name '{process_name}'.")
'''