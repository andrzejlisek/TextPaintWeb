#include "editorinfo.h"

EditorInfo::EditorInfo()
{
    Info0.Add("Common keys:");
    Info0.Add("  Arrows, Home, End, PageUp, PageDown - Move cursor.");
    Info0.Add("  Tab - Cursor display style.");
    Info0.Add("  F1, F2, F3, F4 - Change state or display information.");
    Info0.Add("  F6 - Text writing direction.");
    Info0.Add("  F7 - Save file.");
    Info0.Add("  F8 - Load file.");
    Info0.Add("  F9 - Character selector.");
    Info0.Add("  F10 - File manager.");
    Info0.Add("  F12 - Exit (return to main menu).");
    Info0.Add("  Z - Undo (not works in state 1 and 2).");
    Info0.Add("  X - Redo (not works in state 1 and 2).");
    Info0.Add("");
    Info0.Add("To undo or redo in state 1 or 2:");
    Info0.Add("  1. Switch temporarily to state 3 or 4.");
    Info0.Add("  2. Press Z or X key to undo or redo.");
    Info0.Add("  3. Switch back to state 1 or 2.");
    Info0.Add("");

    Info1.Add("State 1 - Write text:");
    Info1.Add("  Backspace - Move cursor in opposite direction.");
    Info1.Add("  Insert - Insert line or character.");
    Info1.Add("  Delete - Delete line or character.");
    Info1.Add("  Enter - Change Insert/Delete mode.");
    Info1.Add("  Any alphanumeric key - Write key character.");

    Info2.Add("State 2 - Write characters:");
    Info2.Add("  Backspace - Move cursor in opposite direction.");
    Info2.Add("  Insert - Insert line or character.");
    Info2.Add("  Delete - Delete line or character.");
    Info2.Add("  Enter - Change Insert/Delete mode.");
    Info2.Add("  Any alphanumeric key - Write favorite character asigned to key.");

    Info3.Add("State 3 - Character paint:");
    Info3.Add("  W,S,A,D - Change figure size.");
    Info3.Add("  Q,E - Change cursor position within the rectangle or diamond.");
    Info3.Add("  1 - Change figure shape.");
    Info3.Add("  2 - Change character set.");
    Info3.Add("  3 - Draw hollow frame.");
    Info3.Add("  4 - Draw filled frame.");
    Info3.Add("  5 - Start or stop drawing line.");
    Info3.Add("  6,7 - Font and cursor width.");
    Info3.Add("  8,9 - Font and cursor height.");
    Info3.Add("  C - Copy the text.");
    Info3.Add("  V - Paste the text.");
    Info3.Add("  Insert - Insert line or character.");
    Info3.Add("  Delete - Delete line or character.");
    Info3.Add("  Enter - Change Insert/Delete mode.");
    Info3.Add("  I or Numpad Plus - Write vertical line.");
    Info3.Add("  K or Numpad Minus - Write horizontal line.");
    Info3.Add("  Space or Numpad 0 - Write selected character.");
    Info3.Add("");
    Info3.Add("Frame corners, tees and crosses assigned to letters or Numpad digits:");
    Info3.Add("");
    Info3.Add("  T---Y---U         T         7---8---9         7");
    Info3.Add("  |   |   |        / \\        |   |   |        / \\");
    Info3.Add("  |   |   |       G   Y       |   |   |       4   8");
    Info3.Add("  |   |   |      / \\ / \\      |   |   |      / \\ / \\");
    Info3.Add("  G---H---J     B   H   U     4---5---6     1   5   9");
    Info3.Add("  |   |   |      \\ / \\ /      |   |   |      \\ / \\ /");
    Info3.Add("  |   |   |       N   J       |   |   |       2   6");
    Info3.Add("  |   |   |        \\ /        |   |   |        \\ /");
    Info3.Add("  B---N---M         M         1---2---3         3");

    Info4.Add("State 4 - Pixel paint:");
    Info4.Add("  W,S,A,D - Move size cursor.");
    Info4.Add("  I,K,J,L - Move picture between cursors.");
    Info4.Add("  1 - Change paint configuration.");
    Info4.Add("  2 - Change default color.");
    Info4.Add("  3 - Draw one pixel or flood fill.");
    Info4.Add("  4 - Draw line.");
    Info4.Add("  5 - Draw rectangle.");
    Info4.Add("  6 - Draw ellipse.");
    Info4.Add("  P - Turn pencil on/off.");
    Info4.Add("  M - Change picture move mode.");
    Info4.Add("  N - Invert colors.");
    Info4.Add("  C - Copy picture.");
    Info4.Add("  V - Paste picture.");
    Info4.Add("  Q,E - Move cursors around rectangle corners.");
    Info4.Add("  R - Change paint color.");
    Info4.Add("  F - Change paint fill option.");

    Info9.Add("");
    Info9.Add("Character and color selector:");
    Info9.Add("  Arrows - Choose character or color.");
    Info9.Add("  PageUp, PageDown - Depends on state:");
    Info9.Add("    Character selector: Flip one page.");
    Info9.Add("    Color selector: Change number of columns for ANSI file load.");
    Info9.Add("  Home, End - Depends on state:");
    Info9.Add("    Character selector: Flip 16 pages.");
    Info9.Add("    Color selector: Change number of rows for ANSI file load.");
    Info9.Add("  F1, F2: - Depends on state:");
    Info9.Add("    Character selector: Switch the plane.");
    Info9.Add("    Color selector: Switch the CR and LF character parsing (3 choices for each).");
    Info9.Add("  F3 - Select between states:");
    Info9.Add("    Character selector: Switch into Color selector.");
    Info9.Add("    Color selector: Switch into Character selector.");
    Info9.Add("  F4 - Depends on state - ANSI process and display parameters.");
    Info9.Add("  Delete - Go to character or color under text cursor.");
    Info9.Add("  Insert - Depends on state:");
    Info9.Add("    Character selector: Switch between ordinary and favorite state.");
    Info9.Add("    Color selector: Toggle drawing text and color (3 choices).");
    Info9.Add("  Backspace (character selector only):");
    Info9.Add("    Ordinary: Select character for save as favorite.");
    Info9.Add("    Favorite: Select character or put character on pointed place.");
    Info9.Add("  Tab - Move character selector window.");
    Info9.Add("  Enter - Change selected character or color and close.");
    Info9.Add("  F9 - Close without changing selected character or color.");
    Info9.Add("  Any alphanumeric key (character selector only):");
    Info9.Add("    Ordinary: Save pointed character into favorite set under pressed key.");
    Info9.Add("    Favorite: Go to place assigned with pressed key.");
    Info9.Add("  Number key (color selector only) - Toggle attribute:");
    Info9.Add("    1 - Bold");
    Info9.Add("    2 - Italic");
    Info9.Add("    3 - Underline");
    Info9.Add("    4 - Strikethrough");
    Info9.Add("    5 - Blink");
    Info9.Add("    6 - Reverse");
    Info9.Add("    7 - Conceale");
}

void EditorInfo::CreateInfo(int N)
{
    Info.Clear();
    Info.AddRange(Info0);
    switch (N)
    {
        case 1: Info.AddRange(Info1); break;
        case 2: Info.AddRange(Info2); break;
        case 3: Info.AddRange(Info3); break;
        case 4: Info.AddRange(Info4); break;
    }
    Info.AddRange(Info9);

    InfoX = 0;
    InfoY = 0;
    InfoW = 20;
    for (int i = 0; i < Info.Count; i++)
    {
        if (InfoW < Info[i].length())
        {
            InfoW = Info[i].length();
        }
    }
    InfoW--;
    InfoH = Info.Count - 1;
}

int EditorInfo::Move(int Dir)
{
    switch (Dir)
    {
        case 0:
            if (InfoY > 0)
            {
                InfoY--;
                return 2;
            }
            break;
        case 1:
            if (InfoY < InfoH)
            {
                InfoY++;
                return 3;
            }
            else
            {
                if (InfoY != InfoH)
                {
                    InfoY = InfoH;
                    return 1;
                }
            }
            break;
        case 2:
            if (InfoX > 0)
            {
                InfoX--;
                return 4;
            }
            break;
        case 3:
            if (InfoX < InfoW)
            {
                InfoX++;
                return 5;
            }
            else
            {
                if (InfoX != InfoW)
                {
                    InfoX = InfoW;
                    return 1;
                }
            }
            break;
    }
    return 0;
}
