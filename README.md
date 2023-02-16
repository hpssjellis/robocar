# robocar
Shredder RC car arduino hacked for machine learning with PortentaH7 and vision shield



Started Feb 12th, 2023



Code should end up here in this folder of my Arduino Library the Portenta Pro Community Solutions.  
If you understand the referencing sytem it is in the "dot55" area.

https://github.com/hpssjellis/portenta-pro-community-solutions/tree/main/examples/dot5-portenta-machine-learning/dot55-robocar




## Main Build steps

1. Purchase parts checkout the price list at [price-list.md](price-list.md)
2. Have fun with the RC car, without breaking it.
3. Find your objects to track. I use double sided black circles on photo copy paper. Cut them out 
4. 3D Print or make with wood or cardboard, some way to hold the portentaH7 with LoRa vision shield on the car along with a usb-C cable with battery pack, attach to the car while it still has the RC componenets working. This allows you to drive the car while taking lots of images on the SD card. You will need this program [dot362-png-to-sd-card.ino](https://github.com/hpssjellis/portenta-pro-community-solutions/blob/main/examples/dot3-portenta-vision-shields/dot36-camera-png-to-web/dot362-png-to-sd-card/dot362-png-to-sd-card.ino) You might want to attch sunglasses to the model if reflections are a problem when using the car inside.

5. EdgeImpulse.com upload and train a FOMO model
6. Download the arduino library and install the .zip file
7. Load the Arduino sketch [dot551-robocar-v1.ino](https://github.com/hpssjellis/portenta-pro-community-solutions/blob/main/examples/dot5-portenta-machine-learning/dot55-robocar/dot551-robocar-v1/dot551-robocar-v1.ino) but loading it is easy from the Arduino IDE installed "Portenta Pro Community Solutions" library sketch dot551 (check for newer versions)
8. If you have the WaveShare grayscale 128 x 128 OLED then attach the correct pins and run the code and abserve your modle if it works.
9. Remove the 5 wire servo that comes with the shredder car and attach the 3 wire servo. The ball joints are a bit difficult to replace.
10. Using a long breadboard attached to the car, connect the Big Motor Drive and Servo motor driver making sure the high car voltage is kept away from the 5 volt USB input Portenta
11. Test that the mechanical parts of the car are working by running this test code [here ./media/a01-motor-testing.ino](./media/a01-motor-testing.ino). This allows you to check the motor slowest speed and best full turning angle. (Don't push the angles too far as the servo motor complains)
12. Once everything is proved to work, upload the main code again, change a few of the variables such as PWM_MIN and maximum turning angles, and try out your car
13. Start editing the code to make it work better
14. Also edit the 3D Printing to add lights, etc.
15. Share your code on social media and @rocksetta 
 







