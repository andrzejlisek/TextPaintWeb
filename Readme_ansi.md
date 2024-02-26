# ANSI in TextPaint

This document describes the general information about ANSI sequence processing and VTTEST test application working\.

# DOS vs\. VTx terminal

Older ANSI files uses features of DOS terminal, which works slightly differently than standard VTx/ANSI terminal\. The differences cannot be accomodate with each other, because the same data sequence \(text with escape codes\) giver different result\. In the table below, there are all differences:

| Feature | VTx/Unix data \- ANSIDOS=0 or ANSIDOS=2 | ANSI/DOS data \- ANSIDOS=1 |
| --- | --- | --- |
| Text wrapping\. | After writing character at the last column, cursor remains at the same line\. The cursor jumps into beginning of next line only after writing one more character and this character will be written at the first column\. | After writing character at the last column, cursor immediately jumps into beginning of next line\. |
| Characters from **00h** to **1Fh**, excluding from **05h**, **07h** to **0Dh**, **1Ah**, **1Bh**\. | Character will be ignored\. | Character will be written using assigned printable character according standard DOS character glyph for control characters\. |
| Character **05h** \- enquiry | Request AnswerBack message | Write character **2663h**\. |
| Character **07h** \- bell | Emit the "bell" sound\. | Write character **2022h**\. |
| Character **08h** \- backspace | Move cursor left one column or ignore\. | Move cursor left one column or write character **25D8h**\. |
| Character **09h** \- horizontal tab | Move cursor right to the nearest multiply of 8 columns or ignore\. | Move cursor right to the nearest multiply of 8 columns or write character **25CBh**\. |
| Character **0Bh** \- vertical tab | Move cursor down one line\. | Write character **2642h**\. |
| Character **0Ch** \- form feed | Same as **0Ah**\. | Write character **2640h**\. |
| Character **7Fh** | Ignore\. | Write character **7Fh**\. |
| Sequence **1Bh D** | Move cursor one line up, scroll if necessary\. | Ignore\. |
| Sequence **1Bh M** | Move cursor one line down, scroll if necessary\. | Enter into music state\. |
| Sequence **1Bh \[ x L** | Scroll **x** lines up\. | Ignore\. |
| Sequence **1Bh \[ x M** | Scroll **x** lines down\. | Enter into music state\. |
| Sequence **1Bh \[ x C** | Move cursor right, if **x** exceedes distance to the last column, leave cursor at the last column\. | Move cursor right, if **x** exceedes distance to the last column, jump to begining of next line and move remaining value from begining of next line\. |
| Sequence **1Bh \[ 2 J** | Clear screen only\. | Clear screen and move cursor to upper left corner\. |
| Character sets | Remap, change is possible | No remap, change is not possible |

## Character set

Original VTx terminals operates on 7\-bit data or 8\-bit data, so the character map consists of 256 items including control and special characters\. At the display, the character can be mapped into one of the several character sets and che character set can be changed during data transmission\. The character management is detailed in the **Readme\_charmap\.md** file\. This feature works only, when **ANSIDOS=0** or **ANSIDOS=2**\.

The only difference between **ANSIDOS=0** and **ANSIDOS=2** is character map assignment by default after reset:


* **ANSIDOS=0**: ASCII for **GL** and ISO Latin\-1 for **GR**\. \- mapping numbers "as is", each output character has the same number as input character, so this assignment matches with ANSI/ASCII, ISO\-8859\-1 data and decoded Unicode data containing LATIN\-1 characters\.
* **ANSIDOS=2**: ASCII for **GL** and DEC Supplemental Graphic for **GR**\. \- mapping characters is the same as in original DEC VTx terminals and is compatible with original VTx demo and test files/software\. Processing other files, includng Unicode and ISO\-8859\-1 files are possible, but some Latin\-1 characters will be displayed incorrectly\.

In both cases, the character map assignment can be changed during data incoming due to special commands, described in the **Readme\_charmap\.md** file\. The difference between **ANSIDOS=0** and **ANSIDOS=2** will not be importand in the following cases:


* In incomping data there are commands, which explicitly selects any character map into **GL**\.
* The data does not contain the characters numbered between 160 and 255 after encoding, wich are different between ISO Latin\-1 and DEC Supplemental Graphic maps\.

For the **ANSIDOS=1**, there is the same default mapping as for the **ANSIDOS=2**, so actually there is not any remapping\. The most DOS files uses IBM 437 code page, which is very different from the ISO\-8859\-1, but the character maps is used for displaying characters between 160 and 256, not for data decoding\. The most output characters from IBM 437 data has the number above 256\. Character map change is not possible in the **ANSIDOS=1**, the original ANSI\.SYS driver does not provide character map change during data processing\.

## Recommended display settings

The difference between DOS and VTx data implies different values for some parameters\. Below there are listed recommended values in most cases, but you can use other values:

| Parameter | Value for VTx/Unix | Value for ANSI/DOS |
| --- | --- | --- |
| ANSIDOS | Usually 0, sometimes may require 2 | 1 |
| ANSI8bit | Usually 0, sometimes may require 1 | 0 |
| ANSIPrintBackspace | 0 | Usually 0, sometimes 1 |
| ANSIPrintTab | 0 | Usually 0, sometimes 1 |
| ANSIReadCR | Usually 0, sometimes 2 | 0 |
| ANSIReadLF | Usually 0, sometimes 1 | 0 |
| DisplayBlink | 1 | 0 or 2 \(depending on data\) |
| DisplayAttrib | 15 | 0 |
| ANSIColors | 1 | 1 |
| ANSIReverseMode | 2 | 1 |
| ANSIColorBold | 0 | 1 |
| ANSIColorBlink | 0 | 0 or 1 \(depending on data\) |
| ANSIIgnoreConcealed | 0 | 0 |

## Music state

Some ANSI files uses ANSI music, which generates simple sounds through PC speaker\. Sequence **1Bh M** or **1Bh \[ x M** informs the application, that following printable characters will define sound sequence and are not to write on the screen\. The last character of such sequence is **0Eh** and demands the application to exit from the music state\. If **03h** or **0Eh** occurs without entering into music state, the character will be printed or ignored according the **ANSIDOS** parameter\.

When **ANSIDOS=0** or **ANSIDOS=2**, the music state is not available\. Generating the sound is not implemented, because there is very rarely used and is not necessary to view or edit ANSI file\.

## DOS control character assignment

Characters from 00h to 1Fh are control characters and should be treated as non\-printable characters\. DOS has ability to print control character in some cases and it was used in some ANSI files\.

| Byte value | Character name | Default printable character |
| --- | --- | --- |
| 00h | NUL | 0020h |
| 01h | SOH | 263Ah |
| 02h | STX | 263Bh |
| 03h | ETX | 2665h |
| 04h | EOT | 2666h |
| 05h | ENQ | 2663h |
| 06h | ACK | 2660h |
| 07h | BEL | 2022h |
| 08h | BS | 25D8h |
| 09h | HT | 25CBh |
| 0Ah | LF | 25D9h |
| 0Bh | VT | 2642h |
| 0Ch | FF | 2640h |
| 0Dh | CR | 266Ah |
| 0Eh | SO | 266Bh |
| 0Fh | SI | 263Ch |
| 10h | DLE | 25BAh |
| 11h | DC1 | 25C4h |
| 12h | DC2 | 2195h |
| 13h | DC3 | 203Ch |
| 14h | DC4 | 00B6h |
| 15h | NAK | 00A7h |
| 16h | SYN | 25ACh |
| 17h | ETB | 21A8h |
| 18h | CAN | 2191h |
| 19h | EM | 2193h |
| 1Ah | SUB | 2192h |
| 1Bh | ESC | 2190h |
| 1Ch | FS | 221Fh |
| 1Dh | GS | 2194h |
| 1Eh | RS | 25B2h |
| 1Fh | US | 25BCh |

The characters from **00h** to **1Fh** from the ANSI file can be printed in exception of **0Ah**, **0Dh**, **1Ah** and **1Bh**\.

The printable character assignment can be changed by **ANSICharsDOS** setting\.

# Escape sequences

TextPaint has implemented limited set of escape sequences used to ANSI data processing and interpreting\. The sequences are following and enough for most files and many telnet services\.

## The non\-standard sequences

The sequences consists of constant number of character in exception by **1Bh \]**, which ends by **07h**\. The **ANSIDOS=0** remark means **ANSIDOS=0** or **ANSIDOS=2**\.

| Escape sequence | XTERM name | Meaning |
| --- | --- | --- |
| 1Bh \( P1 |   | Character set for bank G0, details in **Readme\_charmap\.md** file\. |
| 1Bh \) P1 |   | Character set for bank G1, details in **Readme\_charmap\.md** file\. |
| 1Bh \* P1 |   | Character set for bank G2, details in **Readme\_charmap\.md** file\. |
| 1Bh \+ P1 |   | Character set for bank G3, details in **Readme\_charmap\.md** file\. |
| 1Bh \- P1 |   | Character set for bank G1, details in **Readme\_charmap\.md** file\. |
| 1Bh \. P1 |   | Character set for bank G2, details in **Readme\_charmap\.md** file\. |
| 1Bh / P1 |   | Character set for bank G3, details in **Readme\_charmap\.md** file\. |
| 1Bh Space F | S7C1T | Set the 7\-bit controls for input\. |
| 1Bh Space G | S8C1T | Set the 8\-bit controls for input\. |
| 1Bh n | LS2 | Use character bank 2\. |
| 1Bh o | LS3 | Use character bank 3\. |
| 1Bh c | RIS | Reset terminal\. |
| 1Bh \# 0 |   | Ignore\. |
| 1Bh \# 1 |   | Ignore\. |
| 1Bh \# 2 |   | Ignore\. |
| 1Bh \# 3 | DECDHL | Set current line font to double width and double height, display upper part\. |
| 1Bh \# 4 | DECDHL | Set current line font to double width and double height, display lower part\. |
| 1Bh \# 5 | DECSWL | Set current line font to normal width and normal height\. |
| 1Bh \# 6 | DECDWL | Set current line font to double width and normal height\. |
| 1Bh \# 7 |   | Ignore\. |
| 1Bh \# 8 | DECALN | Fill screen with **E** character\. |
| 1Bh \# 9 |   | Ignore\. |
| 1Bh \] \.\.\. 07h |   | Ignore\. |
| 1Bh 6 | DECBI | Move line left by deleting first character\. |
| 1Bh 7 | DECSC | Save current cursor position and text attributes\. |
| 1Bh 8 | DECRC | Restore saved cursor position and text attributes\. |
| 1Bh 9 | DECFI | Move line right by inserting space before first character\. |
| 1Bh D \- **ANSIDOS=0** | IND | Move cursor down by 1 step, scroll screen if needed\. |
| 1Bh D \- **ANSIDOS=1** |   | Ignore\. |
| 1Bh M \- **ANSIDOS=0** | RI | Move cursor up by 1 step, scroll screen if needed\. |
| 1Bh M \- **ANSIDOS=1** |   | Enter to music state, ignore following characters\. |
| 1Bh E | NEL | Move cursor down by 1 step, scroll screen if needed and move cursor to first column\. |
| 1Bh H | HTS | Add current column to tabulator list |
| 1Bh P | DCS | Switch into DCS state\. Next arriving character will be buffered without printing\. |
| 1Bh \\ | ST | If within DCS state: Exit from DCS state and request DCS report\. Normally, ignore\. |
| 1Bh = | DECKPAM | Change numeric keypad mode to 1\. |
| 1Bh > | DECKPNM | Change numeric keypad mode to 0\. |
| 1Bh N | SS2 | Ignore\. |
| 1Bh O | SS3 | Ignore\. |
| 1Bh V | SPA | Enable char protection against ED, EL and ECH\. |
| 1Bh W | EPA | Disable char protection against ED, EL and ECH\. |
| 05h \- **ANSIDOS=0** | ENQ | Request the AnswerBack message\. |
| 08h | BS | Move cursor one step left\. |
| 09h \- **ANSIDOS=0** | TAB | Move cursor right to nearest multiply of 8 or defined tab stop\. |
| 0Ah | LF | Move cursor one line down, scroll screen if needed\. |
| 0Bh \- **ANSIDOS=0** | VT | Move cursor one line down, scroll screen if needed\. |
| 0Ch \- **ANSIDOS=0** | FF | Move cursor one line down, scroll screen if needed\. |
| 0Dh | CR | Move cursor to first column\. |
| 0Eh \- **ANSIDOS=1** |   | Exit from music state if in music state, otherwise, write character\. |
| 0Eh \- **ANSIDOS=0** | SO | Use character bank 1\. |
| 0Fh \- **ANSIDOS=0** | SI | Use character bank 0\. |
| 1Ah | SUB | Break data processing, ignore in **WorkMode=2**\. |

When **ANSIDOS=1**, every character between **01h** to **1Fh** will be written as standard character in exception of characters: **08h**, **0Ah**, **0Dh**, **1Ah** and **1Bh**\.

When **ANSIDOS=0** or **ANSIDOS=2**, every character between **01h** to **1Fh** will be ignored in exception of characters: **08h**, **09h**, **0Ah**, **0Bh**, **0Dh**, **1Ah** and **1Bh**\.

## The standard sequences

Every standard sequence begins with **1Bh** followed by **\[** character and ends with any letter or one of the characters: **@** **\`** **\{** **\}** **~** **&#124;**\.

The standard sequences with **?** and **h**/**l** charater are the binary options, the options are in the table below\. For example, DECSET / DECOM means 1Bh \[ ? 6 h:

| Parameter | XTERM name | Meaning for DECSET \- 1Bh \[ ? P1 h | Meaning for DECRST \- 1Bh \[ ? P1 l |
| --- | --- | --- | --- |
| 2 | DECANM | None | Enter into VT52 mode\. |
| 3 | DECCOLM | Resize screen to 132 columns with clearing\. | Resize screen to 80 columns with clearing\. |
| 4 | DECSCLM | Enable smooth scrolling\. | Disable smooth scrolling\. |
| 5 | DECSCNM | Enable negative screen\. | Disable negative screen\. |
| 6 | DECOM | Enable origin mode and move cursor to top left corner of cursor area\. | Disable origin mode and move cursor to top left corner of screen\. |
| 7 | DECAWM | Disable screen wrapping\. | Enable screen wrapping\. |
| 9 |   | Enable mouse click events\. | Disable mouse click events\. |
| 25 | DECTCEM | Show cursor\. | Hide cursor\. |
| 42 | DECNRCM | Enable national character replacement\. | Disable national character replacement\. |
| 47 |   | Switch to alternate screen\. | Switch to main screen\. |
| 66 | DECNKM | Set numeric keypad mode to 1\. | Set numeric keypad mode to 0\. |
| 67 | DECBKM | Set backspace key mode to 1\. | Set backspace key mode to 0\. |
| 69 | DECLRMM | Enable left and right margin\. | Disable left and right margin\. |
| 95 | DECNCSM | Preserve screen when DECCOLM executed\. | Clear screen when DECCOLM executed\. |
| 1000 |   | Enable mouse button events\. | Disable mouse button events\. |
| 1001 |   | Enable highlight mouse events\. | Disable highlight mouse events\. |
| 1002 |   | Enable mouse button and drag events\. | Disable mouse button and drag events\. |
| 1003 |   | Enable mouse button and movement events\. | Disable mouse button and movement events\. |
| 1004 |   | Enable window focus event\. | Disable window focus event\. |
| 1005 |   | Enable UTF\-8 mouse data, disable other mouse data\. | Disable UTF\-8 mouse data\. |
| 1006 |   | Enable SGR mouse data, disable other mouse data\. | Disable SGR mouse data\. |
| 1015 |   | Enable URXVT mouse data, disable other mouse data\. | Disable URXVT mouse data\. |
| 1016 |   | Enable SGR\-Pixels mouse data \(single cell is always 8x16 pixels\), disable other mouse data\. | Disable SGR\-Pixels mouse data \(single cell is always 8x16 pixels\)\. |
| 1047 |   | Switch to alternate screen\. | Clear screen and switch to main screen\. |
| 1048 |   | Save cursor\. | Restore cursor\. |
| 1049 |   | Save cursor and switch to alternate screen\. | Clear screen, switch to main screen and restore cursor\. |

The standard sequences may contains parameters\. When parameter is ommited, the default value is **0** or **1**, depending on sequence:

| Escape sequence | XTERM name | Meaning |
| --- | --- | --- |
| 1Bh \[ \! p | DECSTR | Reset terminal state including clearing the screen\. |
| 1Bh \[ s | SCOSC | Save current cursor position and text attributes\. |
| 1Bh \[ u | SCORC | Restore saved cursor position and text attributes\. |
| 1Bh \[ 4 h | SM / IRM | Enable inserting mode\. |
| 1Bh \[ 4 l | RM / IRM | Disable inserting mode\. |
| 1Bh \[ 6 h | SM / ??? | Do not use char protection in ED, EL and ECH commands\. |
| 1Bh \[ 6 l | RM / ??? | Use char protection in ED, EL and ECH commands\. |
| 1Bh \[ 12 h | SM / SRM | Disable local echo \(**WorkMode=2** only\)\. |
| 1Bh \[ 12 l | RM / SRM | Enable local echo \(**WorkMode=2** only\)\. |
| 1Bh \[ 20 h | SM / LNM | Enable new line mode \(**WorkMode=2** only\)\. |
| 1Bh \[ 20 l | RM / LNM | Disable new line mode \(**WorkMode=2** only\)\. |
| 1Bh \[ 0 J | ED | Clear screen from cursor to bottom right corner \(all characters\)\. |
| 1Bh \[ 1 J | ED | Clear screen from top left corner to cursor \(all characters\)\. |
| 1Bh \[ 2 J | ED | Clear screen and move cursor to top left corner of screen \(all characters\)\. |
| 1Bh \[ 0 K | EL | Clear current line from cursor to right edge \(all characters\)\. |
| 1Bh \[ 1 K | EL | Clear current line from left edge to cursor \(all characters\)\. |
| 1Bh \[ 2 K | EL | Clear current line from left edge to right edge \(all characters\)\. |
| 1Bh \[ ? 0 J | DECSED | Clear screen from cursor to bottom right corner \(unprotected characters only\)\. |
| 1Bh \[ ? 1 J | DECSED | Clear screen from top left corner to cursor \(unprotected characters only\)\. |
| 1Bh \[ ? 2 J | DECSED | Clear screen and move cursor to top left corner of screen \(unprotected characters only\)\. |
| 1Bh \[ ? 0 K | DECSEL | Clear current line from cursor to right edge \(unprotected characters only\)\. |
| 1Bh \[ ? 1 K | DECSEL | Clear current line from left edge to cursor \(unprotected characters only\)\. |
| 1Bh \[ ? 2 K | DECSEL | Clear current line from left edge to right edge \(unprotected characters only\)\. |
| 1Bh \[ P1 ; P2 H | CUP | Move cursor to column P1 and line P2, move cursor to be within cursor area\. |
| 1Bh \[ P1 ; P2 f | HVP | Move cursor to column P1 and line P2, move cursor to be within cursor area\. |
| 1Bh \[ P1 A | CUU | Move cursor up through P1 steps\. |
| 1Bh \[ P1 B | CUD | Move cursor down through P1 steps\. |
| 1Bh \[ P1 C | CUF | Move cursor right through P1 steps\. |
| 1Bh \[ P1 D | CUB | Move cursor left through P1 steps\. |
| 1Bh \[ P1 d | VPA | Move cursor to P1 line\. |
| 1Bh \[ P1 e | VPR | Move cursor down through P1 lines\. |
| 1Bh \[ P1 \` | HPA | Move cursor to P1 column\. |
| 1Bh \[ P1 a | HPR | Move cursor right through P1 columns\. |
| 1Bh \[ P1 E | CNL | Move cursor to the first column and P1 lines down\. |
| 1Bh \[ P1 F | CPL | Move cursor to the first column and P1 lines up\. |
| 1Bh \[ P1 G | CHA | Move cursor to P1 column\. |
| 1Bh \[ P1 S | SU | Scroll cursor area P1 times down\. |
| 1Bh \[ P1 T | SD | Scroll cursor area P1 times up\. |
| 1Bh \[ P1 ; P2 r | DECSTBM | Define cursor area as from P1 line to P2 line, move cursor to be within cursor area\. |
| 1Bh \[ P1 ; P2 s | DECSLRM | Define left and right margin\. |
| 1Bh \[ P1 L \- **ANSIDOS=0** | IL | Scroll cursor area P1 times up\. |
| 1Bh \[ P1 L \- **ANSIDOS=1** |   | Ignore\. |
| 1Bh \[ P1 M \- **ANSIDOS=0** | DL | Scroll cursor area P1 times down\. |
| 1Bh \[ P1 M \- **ANSIDOS=1** |   | Enter to music state, ignore following characters\. |
| 1Bh \[ \.\.\. m | SGR | Set attributes, number of parameters can vary\. |
| 1Bh \[ P1 @ | ICH | Insert space and move text being right to be cursor P1 times\. |
| 1Bh \[ P1 P | DCH | Delete character and move text being right to be cursor P1 times\. |
| 1Bh \[ P1 X | ECH | Delete P1 characters right to be cursor without text movement\. |
| 1Bh \[ 0 g | TBC | Remove current column from tabulator list\. |
| 1Bh \[ 3 g | TBC | Clear tabulator list\. |
| 1Bh \[ P1 I | CHT | Forward tabulation P1 times\. |
| 1Bh \[ P1 Z | CBT | Backward tabulation P1 times\. |
| 1Bh \[ P1 b | REP | Repeat last printed character P1 times\. |
| 1Bh \[ P1 Space @ | SL | Move left P1 columns\. |
| 1Bh \[ P1 Space A | SR | Move right P1 columns\. |
| 1Bh \[ P1 ; P2 ; P3 ; P4 ; P5 $r | DECCARA | Set bold or blink or reverse in \(P1,P2,P3,P4\) rectangle or clear bold, blink and reverse in rectangle\. |
| 1Bh \[ P1 ; P2 ; P3 ; P4 ; P5 ; P6 ; P7 ; P8 $v | DECCRA | Copy \(P1,P2,P3,P4\) rectangle to \(P5,P6\)\. |
| 1Bh \[ P1 ; P2 ; P3 ; P4 $z | DECERA | Erase \(P1,P2,P3,P4\) rectangle \(all characters\)\. |
| 1Bh \[ P1 ; P2 ; P3 ; P4 ; P5 $x | DECFRA | Fill in \(P2,P3,P4,P5\) rectangle with P1 character\. |
| 1Bh \[ P1 ; P2 ; P3 ; P4 ; P5 $t | DECRARA | Change bold or blink or reverse in \(P1,P2,P3,P4\) rectangle to opposite state\. |
| 1Bh \[ P1 ; P2 ; P3 ; P4 $\{ | DECSERA | Erase \(P1,P2,P3,P4\) rectangle \(unprotected characters only\)\. |
| 1Bh \[ P1 " q | DECSCA | Enable \(P1=1\) or disable \(P1<>1\) character protection against DECSED, DECSEL, DECSERA\. |
| 1Bh \[ 1 $ \} | DECSASD | Enter in "status bar" mode, character printing and CUP, HVP, SGR, HPA, HPR will be ignored\. |
| 1Bh \[ 0 $ \} | DECSASD | Exit from "status bar" mode\. |
| 1Bh \[ P1 ' \} | DECIC | Insert P1 columns\. |
| 1Bh \[ P1 ' ~ | DECDC | Delete P1 columns\. |
| 1Bh \[ P1 ; P2 " p | DECSCL | Set the 7\-bit controls for input when P2=1, otherwise set 8\-bit controls\. |
| 1Bh \[ P1 \* &#124; | DECSNLS | Resize screen to P1 lines\. |
| 1Bh \[ P1 t | DECSLPP | Resize screen to P1 lines, where P1 >= 24\. |
| 1Bh \[ 8 ; P1 ; P2 t | XTWINOPS | Change resolution to P2 x P1 in characters\. |
| 1Bh \[ 4 ; P1 ; P2 t | XTWINOPS | Change resolution to P2 x P1 in pixels\. |

## The request\-response sequences \(reports\)

Some sequences does not changing the terminal working, but received from server in **WorkMode=2**, induces response to server \(asterisk indicates command, which response varies depending on **TerminalType** parameter\):

| Request | XTERM name | Response \(for VT102\) |
| --- | --- | --- |
| 1Bh \[ 0 c | Primary DA \* | 1Bh \[ ? 6 c |
| 1Bh \[ 5 n | DSR | 1Bh \[ 0 n |
| 1Bh \[ 6 n | DSR / CPR | 1Bh \[ YY ; XX R where YY and XX are current cursor position |
| 1Bh \[ ? 6 n | DSR / DECXCPR | 1Bh \[ YY ; XX ; 1 R where YY and XX are current cursor position |
| 1Bh \[ ? 1 5 n | DSR / Printer | 1Bh \[ ? 1 3 n |
| 1Bh \[ ? 2 5 n | DSR / UDK | 1Bh \[ ? 2 0 n |
| 1Bh \[ ? 2 6 n | DSR / Keyboard | 1Bh \[ ? 2 7 ; 1 ; 0 ; 0 n |
| 1Bh \[ ? 5 3 n | DSR / Locator | 1Bh \[ ? 5 3 n |
| 1Bh \[ ? 6 2 n | DSR / DECMSR | 1Bh \[ 0 0 0 0 \* \{ |
| 1Bh \[ ? 6 3 ; 1 n | DSR / DECCKSR | 1Bh P 1 \! ~ 0 0 0 0 1Bh \\ |
| 1Bh \[ ? 7 5 n | DSR / Data integrity | 1Bh \[ ? 7 0 n |
| 1Bh \[ ? 8 5 n | DSR / Multi\-session | 1Bh \[ ? 8 3 n |
| 1Bh \[ > 0 c | Secondary DA \* | 1Bh \[ > 0 ; 1 0 ; 0 c |
| 1Bh \[ = 0 c | Tertiary DA | 1Bh P \! &#124; 0 0 0 0 0 0 0 0 1Bh \\ |
| 1Bh \[ 0 x | DECREQTPARM | 1Bh \[ 2 ; 1 ; 1 ; 1 1 2 ; 1 1 2 ; 1 ; 0 x |
| 1Bh \[ 1 x | DECREQTPARM | 1Bh \[ 3 ; 1 ; 1 ; 1 1 2 ; 1 1 2 ; 1 ; 0 x |
| 1Bh $ q " p | DCS / DECSCL \* | 1Bh P 1 $ r P1 ; P2 " p 1Bh \\ where P2=0 when 8\-bit controls, otherwise P2=1 |
| 1Bh $ q \* &#124; | DCS / DECSNLS | 1Bh P 1 $ r P1 \* &#124; 1Bh \\ where P1 is the number of lines |
| 1Bh \[ ? P1 $ p | DECRQM / DECLRMM | 1Bh \[ ? P1 ; VAL $ y where VAL is from 0 to 4, depending on P1 value and current state |
| 1Bh P $ q r 1Bh \\ | DECRQSS / DECSTBM | 1Bh P 1 $ r P1 ; P2 r 1B \\ where P1 and P2 are the first and last line |
| 1Bh P $ q s 1Bh \\ | DECRQSS / DECSLRM | 1Bh P 1 $ r P1 ; P2 s 1B \\ where P1 and P2 are the first and last column |
| 05h |   | AnswerBack message |
| 1Bh P1 ; P2 ; P3 ; P4 ; P5 ; P6 \* y | DECRQCRA | 1Bh P P1 \! ~ 0 0 0 0 1Bh \\ |

## TerminalType\-affected request\-response sequences

There are three sequences, which has response depending on **TerminalType** parameter:

| TerminalType | Primary DA | Secondary DA | DCS / DECSCL |
| --- | --- | --- | --- |
| 0 \- VT100 | 1Bh \[ ? 1 ; 2 c | 1Bh \[ > 0 ; 1 0 ; 0 c | 1Bh P 1 $ r 6 1 ; 1 " p 1Bh \\ |
| 1 \- VT102 | 1Bh \[ ? 6 c | 1Bh \[ > 0 ; 1 0 ; 0 c | 1Bh P 1 $ r 6 1 ; 1 " p 1Bh \\ |
| 2 \- VT220 | 1Bh \[ ? 6 2 ; 1 ; 2 ; 6 ; 7 ; 8 ; 9 c | 1Bh \[ > 1 ; 1 0 ; 0 c | 1Bh P 1 $ r 6 2 ; 1 " p 1Bh \\ |
| 3 \- VT320 | 1Bh \[ ? 6 2 ; 1 ; 2 ; 6 ; 7 ; 8 ; 9 c | 1Bh \[ > 2 4 ; 1 0 ; 0 c | 1Bh P 1 $ r 6 3 ; 1 " p 1Bh \\ |
| 4 \- VT420 | 1Bh \[ ? 6 4 ; 1 ; 2 ; 6 ; 7 ; 8 ; 9 ; 1 5 ; 1 8 ; 2 1 c | 1Bh \[ > 4 1 ; 1 0 ; 0 c | 1Bh P 1 $ r 6 4 ; 1 " p 1Bh \\ |
| 5 \- VT520 | 1Bh \[ ? 6 5 ; 1 ; 2 ; 6 ; 7 ; 8 ; 9 ; 1 5 ; 1 8 ; 2 1 c | 1Bh \[ > 6 4 ; 1 0 ; 0 c | 1Bh P 1 $ r 6 5 ; 1 " p 1Bh \\ |

## The same\-meaning escape sequences

There are other supported escape sequences, which are the same meaining as standard escape sequences:

| Sequence | Equivalent |
| --- | --- |
| 1Bh \[ H | 1Bh \[ 1 ; 1 H |
| 1Bh \[ J | 1Bh \[ 0 J |
| 1Bh \[ K | 1Bh \[ 0 K |
| 1Bh \[ c | 1Bh \[ 0 c |
| 1Bh \[ > c | 1Bh \[ > 0 c |
| 1Bh \[ = c | 1Bh \[ = 0 c |
| 1Bh \[ g | 1Bh \[ 0 g |

## The TextPaint escape sequences

There is escape sequences usable in TextPaint only and should be ignored in other software terminals:

| Sequence | Meaning |
| --- | --- |
| 1Bh \[ 0 ; P1 ; P2 V | Set font width to P1 and font height to P2, the P1 and P2 are not explicity font size, it encodes the font size and character part number\. |
| 1Bh \[ 1 ; P1 V | Wait \(process some dummy steps\) to be processed P1 multiplied by defined constant\. It can be treated as time marker\. Used for terminal recording and playing\. |
| 1Bh \[ 2 ; P1 ; P2 ; \.\.\. ; P49 V | Custom color palette, used while XBIN playback\. The P2\-P49 is the palette consisting of 16 colors in RGB order \(48 numbers\)\. |
| 1Bh \[ 3 ; P1 ; P2 V | Custom font glyph set, used while XBIN playback\. The P1 is the custom font creation action and P2 varies depending of P1 value\. |

### Font size command

The font size dimension \(width or height\) defines simultaneiusly character size \(number of modules\) and displayed character part \(horizontally and vertically\):


* **0** \- size 1, whole character\.
* **1** \- size 2, first part\.
* **2** \- size 2, second part\.
* **3** \- size 3, first part\.
* **4** \- size 3, second part\.
* **5** \- size 3, third part\.
* **6** \- size 4, first part\.
* **7** \- size 4, second part\.
* **8** \- size 4, third part\.
* **9** \- size 4, fourth part\.

The maximum supported font size is 32, so the maximum possible number is 527, which means font size 32 and displayed 32th part\. Currently, **TextPaint** can read and write file with various font sizes\.

### Custom font command

When you load XBIN file, the file will be automatically converted into ANSI character stream and may have special commands for create custom color palette and custom font\. The custom font is more complex and consists of three steps:


* **1Bh \[ 3 ; 1 ; P2 V** \- Prepare blank custom font with sile of **P2** value\.
* **1Bh \[ 3 ; 2 ; P2; P3; P4; \.\.\. ; P9 V** \- Send character glyph definition:
  * **P2** \- Character number from **0** to **255**\.
  * **P4\.\.P9** \- Character glyph definition sequence, the sequence length depends on character size\.
* **1Bh \[ 3 ; 3 V** \- Finish custom font prepare\.

## Text attributes

During processing ANSI data, there will be used internal text attributes with following default values:


* Foreground = \-1 \(means color not defined\)
* Background = \-1 \(means color not defined\)
* Bold = false
* Italic = false
* Underline = false
* Blink = false
* Reverse = false
* Conceale = false
* Strikethrough = false

The attributes can be changed with the **1Bh \[ \.\.\. m** escape sequence\. The number of parameters can vary and has following meaning:

| Parameter | Meaning |
| --- | --- |
| 0 | Set all parameters to default values |
| 00 | Set all parameters to default values |
| 39 | Foreground = \-1 |
| 30 | Foreground = 0 |
| 31 | Foreground = 1 |
| 32 | Foreground = 2 |
| 33 | Foreground = 3 |
| 34 | Foreground = 4 |
| 35 | Foreground = 5 |
| 36 | Foreground = 6 |
| 37 | Foreground = 7 |
| 90 | Foreground = 8 |
| 91 | Foreground = 9 |
| 92 | Foreground = A |
| 93 | Foreground = B |
| 94 | Foreground = C |
| 95 | Foreground = D |
| 96 | Foreground = E |
| 97 | Foreground = F |
| 49 | Background = \-1 |
| 40 | Background = 0 |
| 41 | Background = 1 |
| 42 | Background = 2 |
| 43 | Background = 3 |
| 44 | Background = 4 |
| 45 | Background = 5 |
| 46 | Background = 6 |
| 47 | Background = 7 |
| 100 | Background = 8 |
| 101 | Background = 9 |
| 102 | Background = A |
| 103 | Background = B |
| 104 | Background = C |
| 105 | Background = D |
| 106 | Background = E |
| 107 | Background = F |
| 1 | Bold = true |
| 22 | Bold = false |
| 3 | Italic = true |
| 23 | Italic = false |
| 4 | Underline = true |
| 24 | Underline = false |
| 5 | Blink = true |
| 25 | Blink = false |
| 7 | Reverse = true |
| 27 | Reverse = false |
| 8 | Conceale = true |
| 28 | Conceale = false |
| 9 | Strikethrough = true |
| 29 | Strikethrough = false |
| 38 followed by 5 | Set the foreground color to the next number \(after **5**\)\. |
| 48 followed by 5 | Set the background color to the next number \(after **5**\)\. |
| 38 followed by 2 | Set the foreground color to the RGB from next three numbers \(after **2**\)\. |
| 48 followed by 2 | Set the background color to the RGB from next three numbers \(after **2**\)\. |

The parameters **38** or **48** followed by **5** sets the color from the 256\-color palette\. The parameters **38** or **48** followed by **2** sets the 24\-bit RGB color\. The **TextPaint** supports standard 16\-colors only, so every color from 256\-color paette and every 24\-bit color are mapped into 16 standard color as following:


1. Compute the **HSL** color model \(hue, saturation, lightness\) values\.
2. If the **L** value is lover than **ANSIColorThresholdBlackWhite**, use the black \(color 0\) and finish the procedure\.
3. If the **L** value is higher than **255\-ANSIColorThresholdBlackWhite**, use the white \(color 15\) and finish the procedure\.
4. If the **L** is lower than 128, the gray color is color 8, and saturated colors are the colors from 1 to 6\.
5. If the **L** is higher than 127, the gray color is color 7, and saturated colors are the colors from 9 to 14\.
6. If the **S** value equals to **ANSIColorThresholdGray**, or is lower than **ANSIColorThresholdGray**, set the gray \(color 7 or 8 depending on **L**\) and finish the procedure\.
7. If the **S** value is greater than **ANSIColorThresholdGray**, use one of the saturated colors \(colors from 1 to 6 or from 9 to 14\), choose the color depending on the **H** value\.

If background or foreground is not defined \(is set as **\-1**\), the colors from **ColorNormal** parameter will be used\.

# The VT52 mode

The terminal and ANSI parser has VT52 mode, which can be entered by **1Bh \[ ? 2 l** sequence\. In this mode, there are another escape sequence set\. Each sequence concsists of 2 character with some exceptions:

| Sequence | Meaning |
| --- | --- |
| 1Bh F | Enable semigraphic character set \(different from standard VTx\)\. |
| 1Bh G | Disable semigraphic character set \(different from standard VTx\)\. |
| 1Bh < | Exit from VT52 mode\. |
| 1Bh A | Move cursor up\. |
| 1Bh B | Move cursor down\. |
| 1Bh C | Move cursor right\. |
| 1Bh D | Move cursor left\. |
| 1Bh H | Move cursor to upper left corner\. |
| 1Bh Y XX YY | Move cursor to XX column and YY row, when XX and YY are single character\. |
| 1Bh d | Clear the screen part from upper left corner to cursor\. |
| 1Bh J | Clear the screen part from cursor to lower right corner\. |
| 1Bh E | Move cursor to upper left corner and clear whole screen\. |
| 1Bh Z | Request for **1Bh / Z** response, only in **WorkMode=3**\. |
| 1Bh I | Move cursor up or scroll screen backward\. |
| 1Bh K | Clear current line from cursor to end\. |
| 1Bh b P1 | Background color \(not implemented\)\. |
| 1Bh c P1 | Foreground color \(not implemented\)\. |

Every other sequence will be ignored and treated as one\-character sequence\.

# VTTEST internal application

The VTTEST application is slightly modified vaesion 20230924 of original [https://invisible\-island\.net/vttest/](https://invisible-island.net/vttest/ "https://invisible-island.net/vttest/") source\.

The modificaton includes \(there is not exhaustive list\):


* Convert C source into C\+\+ source\.
* Execute application in additional background thread\.
* Create `fakeio` class, which simulates original I/O functions such printing text\.
* Simulate I/O streams by queues\.
* Remove all system I/O refereces and replace all system functions with appropriate static functions `fakeio` class\.
* Change log file to logging in browser console\.

The modification is as similar to original source as possible for allow to compare with original source\.

You can connect to VTTEST even, if the WebApi server is not available\. There is onlu connection possible without WebApi server\. The main VTTEST puspose is test VT100 compatibility ans simulate terminal connection without real connection\.

# VTTEST compatibility

VTEST is popular VTxxx terminal test application\. Some tests uses features, which is not supported and mentioned as limitations \(80/320 columns switch\), such test also as denotes as passed\. You can run the VTTEST with appropriate parameter to avoid screen gliches, when is should be in other resolution, as is:

```
vttest 24x80.80
```

There is the TextPaint compatibility with VTTEST v2\.7 from 2023\-09\-24\. Some features, which will not need to bo compatible and usable, are not tested\.

The results are the followin meaning:


* **Pass** \- The test passed\.
* **Fail** \- The test failed due to lack of implementation, but the implementation is potentially possible\.
* **Impossible** \- The test failed, because the test uses features, which conflicts with the TextPaint work assumptions\.
* **Partial** \- The test has several subtest and not all subtests passed\.
* **No glitches** \- The test failed, but not generated any glitched and potentially there is not a problem with real software work\.
* **Unavailable** \- The test is exposed in menu, but not implemented in this version of VTTEST\.

| Number | Name | Result | Remarks |
| --- | --- | --- | --- |
| 1 | Cursor movements | Pass | Uses 80/132 column switching\. |
| 2 | Screen features | Pass | Uses 80/132 column switching\. |
| 3 | Character sets |   |   |
| 3\.8 | VT100 Character Sets | Pass |   |
| 3\.9 | Shift In/Shift Out \(SI/SO\) | Pass | Not all character sets are implemented\. |
| 3\.10 | VT220 Locking Shifts | Pass | Not all character sets are implemented, requires NONE codec\. |
| 3\.11 | VT220 Single Shifts | Pass | Not all character sets are implemented, requires NONE codec\. |
| 3\.12 | Soft Character Sets | Unavailable |   |
| 3\.13 | Keyboard Layout with G0 Selection | Pass |   |
| 4 | Double\-sized characters | Pass | Uses 80/132 column switching\. |
| 5 | Keyboard |   |   |
| 5\.1 | LED Lights | Fail / No glitches | Keyboard LEDs are not controlled\. |
| 5\.2 | Auto Repeat | Fail / No glitches | Auto\-repeat key depends on operating system features\. |
| 5\.3 | Keyboard Layout | Pass |   |
| 5\.4 | Cursor Keys | Pass |   |
| 5\.5 | Numeric Keypad | Pass | In "Application mode", you have to use "\." as dot and "\+" as "comma" key\. |
| 5\.6 | Editing Keypad | Normal: pass, VT100/VT52: fail | Sends key codes also in VT52 and VT100 modes\. |
| 5\.7 | Function Keys | Normal: pass, VT100/VT52: fail | Does not send appropriate code in VT52 and VT100 modes\. |
| 5\.8 | AnswerBack | Pass | Changing the AnswerBack message during session is not possible\. |
| 5\.9 | Control Keys | Pass | Use "Escape key" for input all control keys\. |
| 6 | Terminal reports |   |   |
| 6\.1 | AnswerBack Message | Pass |   |
| 6\.2 | Set/Reset Mode \- LineFeed / Newline | Pass |   |
| 6\.3 | Device Status Report \(DSR\) | Pass |   |
| 6\.4 | Primary Device Attributes \(DA\) | Pass |   |
| 6\.5 | Secondary Device Attributes \(DA\) | Pass |   |
| 6\.6 | Tertiary Device Attributes \(DA\) | Pass |   |
| 6\.7 | Request Terminal Parameters \(DECREQTPARM\) | Pass |   |
| 7 | VT52 mode | Pass | Recognises DECSCL and S8C1T everytime\. |
| 8 | VT102 features | Pass | Uses 80/132 column switching\. |
| 9 | Known bugs |   |   |
| 9\.1 | Smooth scroll to jump scroll | Pass |   |
| 9\.2 | Scrolling region | Pass |   |
| 9\.3 | Wide to narrow screen | Pass |   |
| 9\.4 | Narrow to wide screen | Pass | Uses 80/132 column switching\. |
| 9\.5 | Cursor move from double\-wide to single\-wide line | Pass | Uses 80/132 column switching\. |
| 9\.6 | Column mode escape sequence | Pass | Uses 80/132 column switching\. |
| 9\.7 | Wrap around with cursor addressing | Pass |   |
| 9\.8 | Erase right half of double width lines | Pass |   |
| 9\.9 | Funny scroll regions | Pass | The \(0;1\) scroll region is interpreted as \(1;1\) scroll region\. |
| 10 | Reset and self\-test |   |   |
| 10\.1 | Reset to Initial State \(RIS\) | Pass |   |
| 10\.2 | Invoke Terminal Test \(DECTST\) | Pass | The built\-in confidence test not exists and is not usable in emulators\. |
| 10\.3 | Soft Terminal Reset \(DECSTR\) | Pass |   |
| 11 | Non\-VT100 terminals |   |   |
| 11\.1 | VT220 features |   |   |
| 11\.1\.1 | Reporting functions |   |   |
| 11\.1\.1\.1 | Device Status Report \(DSR\) |   |   |
| 11\.1\.1\.1\.1 | Keyboard Status | Pass |   |
| 11\.1\.1\.1\.2 | Operating Status | Pass |   |
| 11\.1\.1\.1\.3 | Printer Status | Pass |   |
| 11\.1\.1\.1\.4 | UDK Status | Pass |   |
| 11\.1\.2 | Screen\-display functions |   |   |
| 11\.1\.2\.1 | Send/Receive mode \(SRM\) | Pass |   |
| 11\.1\.2\.2 | Visible/Invisible Cursor \(DECTCEM\) | Pass |   |
| 11\.1\.2\.3 | Erase Char \(ECH\) | Pass |   |
| 11\.1\.2\.4 | Protected\-Areas \(DECSCA\) | Pass |   |
| 11\.1\.3 | 8\-bit controls \(S7C1T/S8C1T\) | Pass | Requires NONE codec\. |
| 11\.1\.4 | Printer \(MC\) | Fail | The printer features are not supported\. |
| 11\.1\.5 | Soft Character Sets \(DECDLD\) | Impossible |   |
| 11\.1\.6 | Soft Terminal Reset \(DECSTR\) | Pass |   |
| 11\.1\.7 | User\-Defined Keys \(DECUDK\) | Fail |   |
| 11\.2 | VT320 features |   |   |
| 11\.2\.2 | Cursor\-movement |   |   |
| 11\.2\.2\.1 | Pan down \(SU\) | Pass |   |
| 11\.2\.2\.2 | Pan up \(SD\) | Pass |   |
| 11\.2\.2\.3 | Vertical Cursor Coupling \(DECVCCM\) | Unavailable |   |
| 11\.2\.2\.4 | Horizontal Cursor Coupling \(DECPCCM\) | Unavailable |   |
| 11\.2\.3 | Page\-format controls |   |   |
| 11\.2\.3\.1 | Set columns per page \(DECSCPP\) | Pass | Uses 80/132 column switching\. |
| 11\.2\.3\.2 | Set lines per page \(DECSLPP\) | Pass | Changes number of screen lines\. |
| 11\.2\.4 | Page\-movement controls |   |   |
| 11\.2\.4\.1 | Next Page \(NP\) | Unavailable |   |
| 11\.2\.4\.2 | Preceding Page \(PP\) | Unavailable |   |
| 11\.2\.4\.3 | Page Position Absolute \(PPA\) | Unavailable |   |
| 11\.2\.4\.4 | Page Position Backward \(PPB\) | Unavailable |   |
| 11\.2\.4\.5 | Page Position Relative \(PPR\) | Unavailable |   |
| 11\.2\.5 | Reporting functions |   |   |
| 11\.2\.5\.2 | Device Status Report \(DSR\) | Pass |   |
| 11\.2\.5\.3 | Presentation State Reports | Fail |   |
| 11\.2\.5\.4 | Terminal State Reports | Fail |   |
| 11\.2\.5\.5 | User\-Preferred Supplemental Set \(DECAUPSS\) | Fail |   |
| 11\.2\.5\.6 | Window Report \(DECRPDE\) | Fail |   |
| 11\.2\.6 | Screen\-display functions |   |   |
| 11\.2\.6\.2 | Status line \(DECSASD/DECSSDT\) |   |   |
| 11\.2\.6\.2\.1 | Simple Status line | Fail / No glitches | Status bar is not implemented and status bar commands are ignored\. |
| 11\.2\.6\.2\.2 | Graphic\-Rendition in Status line | Fail / No glitches | Status bar is not implemented and status bar commands are ignored\. |
| 11\.2\.6\.2\.3 | Cursor\-movement in Status line | Fail / No glitches | Status bar is not implemented and status bar commands are ignored\. |
| 11\.3 | VT420 features |   |   |
| 11\.3\.2 | Cursor\-movement |   |   |
| 11\.3\.2\.7 | Back Index \(DECBI\) | Pass |   |
| 11\.3\.2\.8 | Forward Index \(DECFI\) | Pass |   |
| 11\.3\.2\.9 | Cursor movement within margins | Pass |   |
| 11\.3\.2\.10 | Other movement \(CR/HT/LF/FF\) within margins | Pass | Requires **ANSIReadCR=0** and **ANSIReadLF=0**\. |
| 11\.3\.3 | Editing sequences |   |   |
| 11\.3\.3\.6 | DECRQM response for DECLRMM | Pass |   |
| 11\.3\.3\.7 | DECRQSS response for DECSTBM | Pass |   |
| 11\.3\.3\.8 | DECRQSS response for DECSLRM | Pass |   |
| 11\.3\.3\.9 | Insert/delete column \(DECIC, DECDC\) | Pass |   |
| 11\.3\.3\.10 | Vertical scrolling \(IND, RI\) | Pass |   |
| 11\.3\.3\.11 | Insert/delete line \(IL, DL\) | Pass |   |
| 11\.3\.3\.12 | insert/delete char \(ICH, DCH\) | Pass |   |
| 11\.3\.3\.13 | ASCII formatting \(BS, CR, TAB\) | Pass | Requires **ANSIReadCR=0** and **ANSIReadLF=0**\. |
| 11\.3\.4 | Keyboard\-control |   |   |
| 11\.3\.4\.1 | Backarrow key \(DECBKM\) | Pass |   |
| 11\.3\.4\.2 | Numeric keypad \(DECNKM\) | Pass |   |
| 11\.3\.4\.3 | Keyboard usage \(DECKBUM\) | Fail |   |
| 11\.3\.4\.4 | Key position \(DECKPM\) | Fail |   |
| 11\.3\.4\.5 | Enable Local Functions \(DECELF\)  | Unavailable |   |
| 11\.3\.4\.6 | Local Function\-Key Control \(DECLFKC\) | Unavailable |   |
| 11\.3\.4\.7 | Select Modifier\-Key Reporting \(DECSMKR\) | Unavailable |   |
| 11\.3\.5 | Macro\-definition \(DECDMAC\) | Unavailable |   |
| 11\.3\.6 | Rectangular area functions |   |   |
| 11\.3\.6\.7 | Change\-Attributes in Rectangular Area \(DECCARA\) | Pass |   |
| 11\.3\.6\.8 | Copy Rectangular area \(DECCRA\) | Pass |   |
| 11\.3\.6\.9 | Erase Rectangular area \(DECERA\) | Pass |   |
| 11\.3\.6\.10 | Fill Rectangular area \(DECFRA\) | Pass |   |
| 11\.3\.6\.11 | Reverse\-Attributes in Rectangular Area \(DECRARA\) | Pass |   |
| 11\.3\.6\.12 | Selective\-Erase Rectangular area \(DECSERA\) | Pass |   |
| 11\.3\.7 | Reporting functions |   |   |
| 11\.3\.7\.2 | Presentation State Reports |   |   |
| 11\.3\.7\.2\.2 | Request Mode \(DECRQM\) / Report Mode \(DECRPM\) |   |   |
| 11\.3\.7\.2\.2\.1 | ANSI Mode Report \(DECRPM\) | Partial / No glitches | Not all reports are supported, but terminal answers for every request\. |
| 11\.3\.7\.2\.2\.2 | DEC Mode Report \(DECRPM\) | Partial / No glitches | Not all reports are supported, but terminal answers for every request\. |
| 11\.3\.7\.2\.3 | Status\-String Report \(DECRQSS\) | Partial / No glitches | Not all reports are supported, but terminal answers for every request\. |
| 11\.3\.7\.3 | Device Status Reports \(DSR\) | Pass | Checksum of rectangular area always returns zero\. |
| 11\.3\.8 | Screen\-display functions |   |   |
| 11\.3\.8\.2 | Select Number of Lines per Screen \(DECSNLS\) | Pass | Changes number of screen lines\. |
| 11\.4 | VT520 features |   |   |
| 11\.4\.2 | Cursor\-movement |   |   |
| 11\.4\.2\.7 | Character\-Position\-Absolute \(HPA\) | Pass |   |
| 11\.4\.2\.8 | Cursor\-Back\-Tab \(CBT\) | Pass |   |
| 11\.4\.2\.9 | Cursor\-Character\-Absolute \(CHA\) | Pass |   |
| 11\.4\.2\.10 | Cursor\-Horizontal\-Index \(CHT\) | Pass |   |
| 11\.4\.2\.11 | Horizontal\-Position\-Relative \(HPR\) | Pass |   |
| 11\.4\.2\.12 | Line\-Position\-Absolute \(VPA\) | Pass |   |
| 11\.4\.2\.13 | Next\-Line \(CNL\) | Pass |   |
| 11\.4\.2\.14 | Previous\-Line \(CPL\) | Pass |   |
| 11\.4\.2\.15 | Vertical\-Position\-Relative \(VPR\) | Pass |   |
| 11\.4\.3 | Editing\-sequences | Unavailable |   |
| 11\.4\.4 | Keyboard\-control | Unavailable |   |
| 11\.4\.5 | Reporting functions |   |   |
| 11\.4\.5\.2 | Presentation State Reports |   |   |
| 11\.4\.5\.2\.2 | Request Mode \(DECRQM\)/Report Mode \(DECRPM\) |   |   |
| 11\.4\.5\.2\.2\.1 | ANSI Mode Report \(DECRPM\) | Partial / No glitches | Not all reports are supported, but terminal answers for every request\. |
| 11\.4\.5\.2\.2\.2 | DEC Mode Report \(DECRPM\) | Partial / No glitches | Not all reports are supported, but terminal answers for every request\. |
| 11\.4\.5\.2\.3 | Status\-String Report \(DECRQSS\) | Partial / No glitches | Not all reports are supported, but terminal answers for every request\. |
| 11\.4\.5\.3 | Device Status Reports \(DSR\) | Pass | Checksum of rectangular area always returns zero\. |
| 11\.4\.6 | Screen\-display functions |   |   |
| 11\.4\.6\.1 | No Clear on Column Change \(DECNCSM\) | Pass | Uses 80/132 column switching\. |
| 11\.4\.6\.2 | Set Cursor Style \(DECSCUSR\) | Fail / No glitches | Cursor is always in steady/blinking underline style\. |
| 11\.5 | ISO\-6429 cursor\-movement |   |   |
| 11\.5\.1 | Character\-Position\-Absolute \(HPA\) | Pass |   |
| 11\.5\.2 | Cursor\-Back\-Tab \(CBT\) | Pass |   |
| 11\.5\.3 | Cursor\-Character\-Absolute \(CHA\) | Pass |   |
| 11\.5\.4 | Cursor\-Horizontal\-Index \(CHT\) | Pass |   |
| 11\.5\.5 | Horizontal\-Position\-Relative \(HPR\) | Pass |   |
| 11\.5\.6 | Line\-Position\-Absolute \(VPA\) | Pass |   |
| 11\.5\.7 | Next\-Line \(CNL\) | Pass |   |
| 11\.5\.8 | Previous\-Line \(CPL\) | Pass |   |
| 11\.5\.9 | Vertical\-Position\-Relative \(VPR\) | Pass |   |
| 11\.6 | ISO\-6429 colors |   |   |
| 11\.6\.2 | Display color test\-pattern | Pass |   |
| 11\.6\.3 | SGR\-0 color reset | Pass |   |
| 11\.6\.4 | BCE\-style clear line/display \(ED, EL\) | Pass |   |
| 11\.6\.5 | BCE\-style clear line/display \(ECH, Indexing\) | Pass |   |
| 11\.6\.6 | VT102\-style features with BCE |   |   |
| 11\.6\.6\.1 | Cursor movements | Pass | Uses 80/132 column switching\. |
| 11\.6\.6\.2 | Screen features | Pass | Uses 80/132 column switching\. |
| 11\.6\.6\.3 | Insert/Delete Char/Line | Pass | Uses 80/132 column switching\. |
| 11\.6\.7 | Other ISO\-6429 features with BCE |   |   |
| 11\.6\.7\.1 | Protected\-Area |   |   |
| 11\.6\.7\.1\.2 | Protected\-Areas \(SPA\) | Pass | ERM is not implemented\. |
| 11\.6\.7\.2 | Repeat \(REP\) | Pass |   |
| 11\.6\.7\.3 | Scroll\-Down \(SD\) | Pass |   |
| 11\.6\.7\.4 | Scroll\-Left \(SL\) | Pass |   |
| 11\.6\.7\.5 | Scroll\-Right \(SR\) | Pass |   |
| 11\.6\.7\.6 | Scroll\-Up \(SU\) | Pass |   |
| 11\.6\.8 | Screen features with BCE | Pass |   |
| 11\.6\.9 | Screen features with ISO 6429 SGR 22\-27 codes | Pass |   |
| 11\.7 | Other ISO\-6429 features |   |   |
| 11\.7\.1 | Protected\-Area Tests |   |   |
| 11\.7\.1\.2 | Protected\-Areas \(SPA\) | Pass |   |
| 11\.7\.2 | Repeat \(REP\) | Pass |   |
| 11\.7\.3 | Scroll\-Down \(SD\) | Pass |   |
| 11\.7\.4 | Scroll\-Left \(SL\) | Pass |   |
| 11\.7\.5 | Scroll\-Right \(SR\) | Pass |   |
| 11\.7\.6 | Scroll\-Up \(SU\) | Pass |   |
| 11\.8 | XTERM special features |   |   |
| 11\.8\.2 | Reporting functions |   |   |
| 11\.8\.2\.2 | Request Mode \(DECRQM\) / Report Mode \(DECRPM\) | Pass | Not all reports are supported, but terminal answers for every request\. |
| 11\.8\.3 | Set window title | Pass |   |
| 11\.8\.4 | Font features |   |   |
| 11\.8\.4\.1 | Modify font | Impossible / No glitches | TextPaint always uses single, specified font\. |
| 11\.8\.4\.2 | Report fonts | Impossible | Font reporting is not supported\. |
| 11\.8\.5 | Mouse features |   |   |
| 11\.8\.5\.3 | X10 Mouse Compatibility | Pass |   |
| 11\.8\.5\.4 | Normal Mouse Tracking | Pass |   |
| 11\.8\.5\.5 | Mouse Highlight Tracking | Pass |   |
| 11\.8\.5\.6 | Mouse Any\-Event Tracking | Pass |   |
| 11\.8\.5\.7 | Mouse Button\-Event Tracking | Pass |   |
| 11\.8\.5\.8 | DEC Locator Events |   |   |
| 11\.8\.5\.8\.1 | One\-Shot | Pass |   |
| 11\.8\.5\.8\.2 | Repeated | Pass |   |
| 11\.8\.5\.8\.3 | One\-Shot \(pixels\) | Pass | Bug in VTTEST: Pixel coordinates are not correctly shown\. |
| 11\.8\.5\.8\.4 | Repeated \(pixels\) | Pass | Bug in VTTEST: Pixel coordinates are not correctly shown\. |
| 11\.8\.5\.8\.5 | Filter Rectangle | Pass | Bug in VTTEST: Rectangle is not drawn correctly\. |
| 11\.8\.5\.8\.6 | Filter Rectangle \(unfiltered\) | Pass |   |
| 11\.8\.5\.8\.7 | XFree86 xterm: screensize | Pass |   |
| 11\.8\.6 | Tektronix 4014 features | Impossible | Graphics is not necessary and not supported\. |
| 11\.8\.7 | Alternate screen features |   |   |
| 11\.8\.7\.3 | Alternate screen \(xterm\) | Pass |   |
| 11\.8\.7\.4 | Improved alternate screen \(XFree86 xterm mode 1047\) | Pass |   |
| 11\.8\.7\.5 | Better alternate screen \(XFree86 xterm mode 1049\) | Pass |   |
| 11\.8\.8 | Window modify\-operations | Partial / Impossible | Resize works, other are not possible and not usable in TextPaint\. |
| 11\.8\.9 | Window report\-operations | Pass | Limited support according assumptions\. |




