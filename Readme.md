# Overview

There is the WebAssembly version of [https://github\.com/andrzejlisek/TextPaint](https://github.com/andrzejlisek/TextPaint "https://github.com/andrzejlisek/TextPaint") project\. There is not simply port\. The source was strongly refactored and mathched into web browser behaviour\. These source are compilable using [https://emscripten\.org/](https://emscripten.org/ "https://emscripten.org/") compiler\.

Now, there is a draft version with lack of some functionality compated than mentioned original TextPaint project\.

Direct link: [https://andrzejlisek\.github\.io/TextPaintWeb/index\.html](https://andrzejlisek.github.io/TextPaintWeb/index.html "https://andrzejlisek.github.io/TextPaintWeb/index.html") 

# Differences

| \.NET version | WebAssembly version \(current state\) |
| --- | --- |
| Reads and writes file directly | Uses internal file storage and files from server |
| Editor: Tho states of character selector by pressing F3 | Three states, the third state is the file list and you can select the file and press Enter\. |
| Terminal: Supports Telnet, SSH, Serial connections | Has embedded VTTEST 2\.7 only\. |
| Run as command with parameters | Has menu displayed at the begin and exit command returns into the menu\. |

# File manager

This version of TextPaint has the internal file manager, which now allows to browse the system files \(**\!\!config\.txt** ans **\!\!system\.txt**\) and the files defined in **\!\!system\.txt** configuration file\. If you modify any file, the file will be stored at local storage and replaces the original default file\.

The file manager is available in editor \(press **F10** key\) and in the animation player \(press **dot/period** key\)\. In the file manager, there are posiible the following keys:


* **Tab** \- Move the popup\.
* **Up arrow** and **Down arrow** \- select item\.
* **Enter** \- Exit from the file manager when selected file or enter into directory or upper directory\.

In editor: You have to press the F8 again to load selected file or press the F7 key to save the current text into the selected file\.

In player\. The file manager is needed for change the current directory\. After closing the file manager, the selected animation will be loaded\.

**Note:** Currently, the file manager has a certain bug: Sometimes, the other files than the two system siles is not visible\. If the problem occurs, just restart the application \(refresh page in browser\)\.

# GUI interface

If you click the screen, there will be show the GUI interface usable both on touchscreen and with mouse\. The button caption represents the current state\. The settings are organized in the following rows:


* **General** \- General actions:
  * **Mouse/Touchscreen** \- Toggle between mouse and touchscreen, this affects on the screen keyboard\.
  * **Restart** \- Restart the application by refresh the page\.
  * **Window/Fullscreen** \- Toggle display between window and fullscreen\.
  * **Return to program** \- Close the interface and return to the program\.
* **Layout** \- Set the screen and virtual keyboard sizes\.
* **Process** \- Toggle the ANSI processing options\. The change affects after processing new data\. In the player, you have to reopen the file\. In the terminal, the will be affect fo new data only\.
* **Colors**, **Font**\- Set the color and font display options\. The result will be shown immediately and the affect the screen display only\.

# To do

This version is not completed yet and now there are need to do following:


* Clean\-up config file\.
* Readme and documentation\.
* Integration with file system and telnet using WebSocket and separated \.NET/Java application\.




