# Project overview

There is the WebAssembly version of [https://github\.com/andrzejlisek/TextPaint](https://github.com/andrzejlisek/TextPaint "https://github.com/andrzejlisek/TextPaint") project, compilable using [https://emscripten\.org/](https://emscripten.org/ "https://emscripten.org/") compiler\. There is not simply port, this project has the following differences \(not exhaustive list\):


* Source was strongly refactored and mathched into web browser behaviour\.
* Internal file storage instead of disk file system\.
* File manager\.
* The config file is renamed to **\!\!config\.txt** to be as first list item, removed other configuration files\.
* Some settings and options are automaticaly saved in **\!\!config\.txt** file\.
* Configuration panel always available\.
* Configuration pop\-up with font and color palette change\.
* Removed text cipher\.
* BIN and XBIN support via internal convert to ANSI file\.
* Internal VTTEST implemented for test and debug purposes\.
* Render is not implemented yet\.

Application: [https://andrzejlisek\.github\.io/TextPaintWeb/index\.html](https://andrzejlisek.github.io/TextPaintWeb/index.html "https://andrzejlisek.github.io/TextPaintWeb/index.html") 

# Description and readme files

TextPaint is not designed to use as plain text editor, there is designed to create or modify text as a simple graphic or scheme using ANSI or Unicode characters\. You can create texts such as organizational charts, simple circuit schemes, algorithm flow, tables, to use it where destination medium supports text printed using fixed\-width font\.

The text can be both monochrome saved as plain text or color saved as text with ANSI escape commands\. You can view or edit many existing ASCII\-art or ANSI\-art files\.

The animation player can play existing ANSI files and terminal can emulate VT100\-compatible terminal\.

The file manager and terminal uses WebApi Server: [https://github\.com/andrzejlisek/WebApiServer](https://github.com/andrzejlisek/WebApiServer "https://github.com/andrzejlisek/WebApiServer")

If you want to use external files or use terminal to connect into any service other than VTTEST, you have to run the **WebApiServer** before running **TextPaint**\. **TextPaint** will connect intto the server automatically\.

If you want to return to the main menu, use exit function in running module or restart **TextPaint** using configuration panel\.

# Documentation

This documentation consists of several readme files:


* **Readme\.md** \- General description, this file\.
* **Readme\_config\.md** \- Configuration interface and **\!\!config\.txt** file contents\.
* **Readme\_files\.md** \- File manager and functions about files\.
* **Readme\_ansi\.md** \- ANSI commands supported by TextPaint, differences betwee modes, and VTTEST compatibility\.
* **Readme\_editor\.md** \- Text editor document\.
* **Readme\_player\.md** \- Animation player document\.
* **Readme\_terminal\.md** \- Terminal document\.
* **Readme\_keyboard\.md** \- Keyboard test document\.
* **Readme\_history\.md** \- Description of important project commits\.




