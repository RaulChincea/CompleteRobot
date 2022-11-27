/*
Links:
https://www.w3schools.com/python/python_file_open.asp
https://www.w3schools.com/python/python_functions.asp
https://pypi.org/project/smbus2/
*/

#include<Wire.h>
#include<Servo.h>

//servo
Servo servo;
int servo_angle = 0;

const int ledPin = 13;

//radio
volatile unsigned long time_read, last_time;
int count;
volatile int channel[10];
int PPM_signal = 3;
int ant_channel_val[10];
int number_of_reads[10];
int checked_val[10]; 
int actual_val[10];
int radio_val;
int ant_radio_val;

int t = 0;
int print_time = 0;
bool channel_check[9];

//I2C
int requested_channel = 1;
int operation = 0; // 0 - read radio, 1 - write servo angle

void setup()
{
    Wire.begin(0x8);
    Serial.begin(9600);

    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    servo.attach(6);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    pinMode(PPM_signal, INPUT);
}

void receiveEvent(int howMany)
{
    while(Wire.available())
    {
        int c = Wire.read();

        if(operation == 1)
        {
          setServo(c);
          operation = 0;
          continue;
        }

        if(c <= 0x8)
        {
          operation = 0;
        }
        else if(c >= 0x9)
        {
          operation = 1;
          continue;
        }

        switch (c)
        {
        case 0x1:
          requested_channel = 1;
          break;
        case 0x2:
          requested_channel = 2;
          break;
        case 0x3:
          requested_channel = 3;
          break;
        case 0x4:
          requested_channel = 4;
          break;
        case 0x5:
          requested_channel = 5;
          break;
        case 0x6:
          requested_channel = 6;
          break;
        case 0x7:
          requested_channel = 7;
          break;
        case 0x8:
          requested_channel = 8;
          break;
        default:
          requested_channel = 1;
          break;
        }

        //Serial.print("CH: ");
        //Serial.println(c);
    }
}

void requestEvent()
{
  int channelVal = channel[requested_channel];

  int first_part = channelVal / 100;
  int second_part = channelVal % 100;

  Wire.write(first_part);
  Wire.write(second_part);

  //Serial.print(first_part);
  //Serial.println(second_part);
}

void loop()
{
    while(channel_check[1] == false || channel_check[2] == false || channel_check[3] == false || channel_check[4] == false || channel_check[5] == false || channel_check[6] == false || channel_check[7] == false || channel_check[8] == false)
    {
        ant_radio_val = radio_val;
        radio_val = digitalRead(PPM_signal);
  
        if(radio_val == 1 && ant_radio_val == 0)
        {
            radio();
        }    
    }

    reset_check();

  //Serial.print("Ch6: ");
  //Serial.println(channel[6]);
}

void radio()
{
  time_read = micros() - last_time;
  last_time = micros();

  count++;

  if(time_read > 5000)
    count = 9;

  switch(count)
  {
    case 1: channel[1] = time_read; channel_check[1] = true; break;
    case 2: channel[2] = time_read; channel_check[2] = true; break;
    case 3: channel[3] = time_read; channel_check[3] = true; break;
    case 4: channel[4] = time_read; channel_check[4] = true; break;
    case 5: channel[5] = time_read; channel_check[5] = true; break;
    case 6: channel[6] = time_read; channel_check[6] = true; break;
    case 7: channel[7] = time_read; channel_check[7] = true; break;
    case 8: channel[8] = time_read; channel_check[8] = true; break;
    default: channel[9] = time_read; count = 0; break; 
  }
}

void reset_check()
{
  for(int i = 1; i <= 8; i++)
  {
    channel_check[i] = false;  
  }
}

void setServo(int servo_angle)
{
  servo.write(servo_angle);
}