# Overview

There is the WebAssembly version of [https://github\.com/andrzejlisek/TextPaint](https://github.com/andrzejlisek/TextPaint "https://github.com/andrzejlisek/TextPaint") project\. There is not simply port\. The source was strongly refactored and mathched into web browser behaviour\. These source are compilable using [https://emscripten\.org/](https://emscripten.org/ "https://emscripten.org/") compiler\.

Now, there is a draft version with lack of some functionality compated than mentioned original TextPaint project\.

Direct link: [https://andrzejlisek\.github\.io/TextPaintWeb/index\.html](https://andrzejlisek.github.io/TextPaintWeb/index.html "https://andrzejlisek.github.io/TextPaintWeb/index.html") 

# Differences

| \.NET version | WebAssembly version \(current state\) |
| --- | --- |
| Reads and writes file directly | Uses internal file storage \- now contains blank file and several demo VT100 animations\. |
| Editor: Tho states of character selector by pressing F3 | Three states, the third state is the file list and you can select the file and press Enter\. |
| Terminal: Supports Telnet, SSH, Serial connections | Has embedded VTTEST 2\.7 only\. |
| Run as command with parameters | Has menu displayed at the begin and exit command returns into the menu\. |

# To do

This version is not completed yet and now there are need to do following:


* Fix ANSI display bugs visible in demo animations and VTTEST \(player and terminal\)\.
* Support for various font sizes, which now are displayed as multiple base size\.
* Smooth scrolling for terminal and player\.
* File contents preservation \(now, the changes will lost after application refresh or exit\)\.
* File import and export\.
* Integration with file system and telnet using WebSocket and separated \.NET/Java application\.
* And much more\.\.\.




