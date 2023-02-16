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

5. EdgeImpulse.com upload and train a FOMO model or grab this one [here ei-ei-v25-fomo-eyes-shades-arduino-1.0.2.zip](https://github.com/hpssjellis/portenta-pro-community-solutions/blob/main/zipped-libraries/edge-Impulse/arduino-exported/Portenta-Camera/ei-ei-v25-fomo-eyes-shades-arduino-1.0.2.zip)
      1. login or register for [Edgeimpulse.com](Edgeimpulse.com)
      2. On the dasho=board far right change (one label to bounding boxes). This makes it a FOMO  (faster objects, more objects) vision model. Also set Target Device to "Arduino PortentaH7 (Cortex M7 H80 MHz)" 
      3. Use the menu connect devices and go to uploader and upload your images that you took of the car driving around your object to detect
      4. Go to menu item Data Aquisiton and click on the "Queue" and start drawing bounding boxes around your objects. "Make sure the label is the same for all of them I use "1eye". The number one in front is very important. 
      5. When all images are labelled, create your standard FOMO model by clicking "create impulse" and choose the standard Edge Impulse image and object detection blocks. Check before saving the impulse that it has only one object.
      6. For the menu item "image" make sure you are saving Grayscale not RGB 
      6. Still in "Image" "Generate Features" wait a few minutes to see the "Feature explorer". Check for weird images and delete or change them
      7. Menu item "Object Detection" set number of training cycles to about 220, whatever works. Add "Data Augmentation" and "start Training" takes about 8 minutes.  
      8. Check your F1 score and confusion matrix, which should be reasonably good hopefully above 80 % background and the 1eye
      9. Check how well it works on your phone or just download it to the Arduino. Reminder this is an Arduino library so you will need to  upload it into the Arduino IDE as a zipped library.
      10. Check the library file name so you can change the library that is in the default program to the new name.
      11. Always place the car on a small box when testing it as it could jump off the table and break.
      12. Flash the code to the Portenta connected to your car. Make sure both batteries are charaged. 
      13. Always unplug both batteries when you have finished testing the car.
      14. Return to Edgeimpulse and try to improve your model.
7. Download the arduino library and install the .zip file
8. Load the Arduino sketch [dot551-robocar-v1.ino](https://github.com/hpssjellis/portenta-pro-community-solutions/blob/main/examples/dot5-portenta-machine-learning/dot55-robocar/dot551-robocar-v1/dot551-robocar-v1.ino) but loading it is easy from the Arduino IDE installed "Portenta Pro Community Solutions" library sketch dot551 (check for newer versions)
9. If you have the WaveShare grayscale 128 x 128 OLED then attach the correct pins and run the code and observe your modle if it works.
10. Remove the 5 wire servo that comes with the shredder car and attach the 3 wire servo. The ball joints are a bit difficult to replace.
11. Using a long breadboard attached to the car, connect the Big Motor Drive and Servo motor driver making sure the high car voltage is kept away from the 5 volt USB input Portenta
12. Test that the mechanical parts of the car are working by running this test code [here ./media/a01-motor-testing.ino](./media/a01-motor-testing.ino). This allows you to check the motor slowest speed and best full turning angle. (Don't push the angles too far as the servo motor complains)
13. Once everything is proved to work, upload the main code again, change a few of the variables such as PWM_MIN and maximum turning angles, and try out your car
14. Start editing the code to make it work better
15. Also edit the 3D Printing to add lights, etc.
16. Share your code on social media and @rocksetta 
 







