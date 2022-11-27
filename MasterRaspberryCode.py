#to stop: channel 6 > 1800

from smbus import SMBus
import time

addr = 0x8
bus = SMBus(1) #indicates /dev/ic2-1
time.sleep(1)

channel = [0, 0, 0, 0, 0, 0, 0, 0, 0]

numb = 1
command = '1'

while channel[6] <= 1800:
    #bus.write_byte(addr, 0x3) #get channel 3
    #read radio
    for i in range(1,9):
        try:
            block = bus.read_i2c_block_data(addr, i, 2) # addr, command, nr of bytes
            value = block[0]*100 + block[1]
        except:
            print("Error reading channel ", i)
        channel[i] = value
        time.sleep(0.01)
    
    #calc servo angle
    if channel[8] < 1000:
        channel[8] = 1000
    if channel[8] > 2000:
        channel[8] = 2000
    servo_angle = int((channel[8]-1000)/5.55)
    print(servo_angle)
    
    #send servo angle
    try:
        bus.write_byte(addr, 0x9)
    except:
        continue
    time.sleep(0.01)
    try:
        bus.write_byte(addr, servo_angle)
    except:
        continue
    #print(channel[1],channel[2],channel[3],channel[4],channel[5],channel[6],channel[7],channel[8])
    


