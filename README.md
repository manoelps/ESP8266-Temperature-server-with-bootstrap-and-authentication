# Arduino-Server-GUI
This is Arduino code to make a server via your home network with the ESP8266 development module.
You can show a temperature from the servermodule by adding a DS18B20 to pin 4

Change the ssid en password to be like your credentials.

Change the username and password to be what you want in the code first
When you go to the IP the serial monitor provides you get greeted by a login page.
When these credentials are correct a cookie will be set with the value of ESPSESSIONID=1, 
when you log out the value will be set to 0, so when you try to acces pages from the server when you're not logged in, 
you can not access the. 

I still have a lot of work and research to do for this project:
I will try to add the following in the next days/weeks:

add database connection
add add user functionality
fill in and watch your profile
add a chat so when multiple users are on the server you can communicate
get omdb api working
make a homepage
add read HTML from sd-card
get temperature from other ESP8266 modules 

