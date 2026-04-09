# ECEn 225 Doorbell Starter Kit

Author: Cameron Saunders

Last Updated: 04-09-26

## Description

An interactive doorbell built on a Raspberry Pi. When pressed, this device takes a picture and uploads it to a server to store images and their timestamps. When the secret menu is activated, this device pulls up a list of all of its files and allows you to view and add filters to the last photo taken.

## Hardware Components

For a comprehensive list of hardware components and how to build the doorbell, please follow this link:

[ECEN 225 Lab 1](https://byu-cpe.github.io/ecen224/labs/01_getting-started/)

---

## Usage Instructions

<!-- This should contain all the instructions needed to access the lab, compile it from source, and run the doorbell. These should be written with the actual commands contained in `` marks (example `sudo ./main` runs the program). If you'd like, include extra details like how to customize certain features or what materials you need to build a doorbell. -->
1. Go to the [Github Repo for the Smart Doorbell](https://github.com/byu-ecen-224-classroom/doorbell-project-Unrelenting-Avarice.git)
2. Clone this repository to your Raspberry Pi.
3. run `sudo ./main` to start the program

When run, the doorbell will pull up a splash page with a welcome message. Pressing the center button will take a picture and send that picture to a private server. When sent, the doorbell will display the image "Ding Dong!" 

On that screen, if you press the code UP UP DOWN DOWN LEFT RIGHT LEFT RIGHT, it will pull up a file viewer menu with more functionality. If the password is wrong, after eight characters it will clear and display "Password cleared". This can also be done by pressing DOWN DOWN DOWN DOWN.

On the secret menu, you can scroll up and down on the files using the button. Pressing left anywhere will take you back to the splash page. Pressing on a filename right will open the file, either as a text document or an image, then return to the menu after 3 seconds. Pressing right on the 'EXIT' file will close the program gracefully and give you a 'Goodbye!' message. Pressing the center button will display "Say Cheese!" and take a picture. It will then allow you to apply filters to this image by presing up, down, left or right. Pressing hte center button again will take yu back to the menu.

---

## Updates & Changelog

### 01/16/26 Getting Started
- Built the hardware for the Pi Z2W

### 02/20/26 Image
- Created filters that filder out red, green, and blue
- Created greyscale and OR filters

### 02/27/26 Display
- Implemented each of the following functions:
    - `clearScreen`: Clears the screen to white
    - `drawHelloWorld`: Draws “hello world” on the screen 10 times in different colors
    - `drawChars`: Draw 10 chars of various values, sizes, colors, and locations onto the screen
    - `drawStars`: Draw at least 20 stars onto the screen
    - `drawFlag`: Uses at least 5 functions from display.h to draw any flag you wish. 3 of the display_draw_### functions must be unique.

- Implemented a menu functionality using `drawMenu`
    - The menu draws the strings contained in the `entries array`
    - When no button is pressed, nothing happens.
    - One entry is selected at a time, which is represented by being highlighted by a blue bar.
    - Pressing the up or down button will change the selected entry.
    - Pressing the right button on the selected entry will run the function with the associated name. For example, the pressed the right button when the entry “Flag” is selected will cause the drawFlag to run. It will wait 2 seconds, then redraw the menu.

- Selection will “wrap” from top to bottom

### 03/06/26 File

- `get_entries` function filters out any file that does not end in .bmp or .log and populates the `entries array` with the the .bmp and .log files
- If the file is a `.bmp` image, display the corresponding image for 2 seconds and then go back to the menu view with the highlight bar over the selected item
- If the file is a `.log` file, display the contents of the file for 2 seconds and then go back to the menu view with the highlight bar over the selected item

### 03/13/26 Camera

- Pressing the center button on the menue will clear the screen, print "Say Cheese!", take a picture with the Pi Z2W’s camera
- Save the raw data received from the capture to a `.bmp`file
- Add the newly saved file to the menu
- Display the contents of the picture when selected from the menu
- While the contents of the picture is showing:

    - If the right button of the d-pad is pressed, filter out the red color of the image
    - If the left button of the d-pad is pressed, filter out the blue color of the image
    - If the up button of the d-pad is pressed, filter out the green color of the image
    - If the down button of the d-pad is pressed, or-filter the image
    - If the center button of the d-pad is pressed, exit from the submenu by clearing the picture and displaying the menu

### 03/27/26 Client

- Send image data to website
- Display log messages to the console ("Socket connected","Image recieved", ect.)

### 04/03/26 Threaded Client

- Create a status bar the size of your highlight bar at the bottom of the screen with a blue background:

    - When the image is being sent in a thread, the text of the bar should say “Sending…” with the bar as a blue background.
    - When your image is sent successfully (i.e. the thread reached the end successfully), change the color of the status bar to green and show “Sent!” as the text

- The status bar will stay green with the sent message for 2 seconds. After the 2 seconds hide the status bar

### 04/15/26 Doorbell

- Automatically run the program when the Raspberry Pi is started.

- When the program starts, it should show a welcome message on the screen
- When the center button is pressed, do the following:

    - In a thread, take a picture, save it to your viewer folder, and send the image to the website.
    - Change the display to show the message to let the user know that doorbell was rung. Make this message visible for 2 seconds and then change back to the initial welcome message.

- Added a hidden menu to your doorbell. When the password is typed, take the user to the file browser menu. To go back to the original welcome message, press the left button of the d-pad.



