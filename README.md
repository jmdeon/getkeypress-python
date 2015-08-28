# getkeypress-python
This small python module written in C allows the user to collect key inputs from user without them having to type into the terminal and press newline.

To compile the python module type command:
make
Once compiled, getkeypress.so will be in your directory, along with the build directory. build/temp* will house the c object files.

Now you can import getkeypress into a python project! 
Example:
import getkeypress as keyboard
keyboard.getkeypress() #this returns a string representing the key a user pressed.
