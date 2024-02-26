#include "terminalkeyboard.h"

TerminalKeyboard::TerminalKeyboard()
{
    TerminalKeys0.Add("Up");
    TerminalKeys0.Add("Down");
    TerminalKeys0.Add("Right");
    TerminalKeys0.Add("Left");
    TerminalKeys1.Add("F1");
    TerminalKeys1.Add("F2");
    TerminalKeys1.Add("F3");
    TerminalKeys1.Add("F4");
    TerminalKeys2.Add("F5");
    TerminalKeys2.Add("F6");
    TerminalKeys2.Add("F7");
    TerminalKeys2.Add("F8");
    TerminalKeys2.Add("F9");
    TerminalKeys2.Add("F10");
    TerminalKeys2.Add("F11");
    TerminalKeys2.Add("F12");
    TerminalKeys3.Add("Home");
    TerminalKeys3.Add("End");
    TerminalKeys4.Add("Insert");
    TerminalKeys4.Add("Delete");
    TerminalKeys4.Add("PageUp");
    TerminalKeys4.Add("PageDown");
    TerminalKeys4.Add("F13");
    TerminalKeys4.Add("F14");
    TerminalKeys4.Add("F15");
    TerminalKeys4.Add("F16");
    TerminalKeys4.Add("F17");
    TerminalKeys4.Add("F18");
    TerminalKeys4.Add("F19");
    TerminalKeys4.Add("F20");

    // Common keys
    TerminalKeys["Escape"] = "1B";
    TerminalKeys["Tab"] = "09";
    TerminalKeys["Enter_0"] = "0D";
    TerminalKeys["Enter_1"] = "0D0A";
    TerminalKeys["Enter_2"] = "0A";
    TerminalKeys["Enter_3"] = "0D0A";
    TerminalKeys["Backspace_0"] = "7F";
    TerminalKeys["Backspace_1"] = "08";

    // Keys depending on configuration 0
    TerminalKeys["Up_000_0"] = "##_[_A";
    TerminalKeys["Down_000_0"] = "##_[_B";
    TerminalKeys["Right_000_0"] = "##_[_C";
    TerminalKeys["Left_000_0"] = "##_[_D";
    TerminalKeys["Up_000_1"] = "##_O_A";
    TerminalKeys["Down_000_1"] = "##_O_B";
    TerminalKeys["Right_000_1"] = "##_O_C";
    TerminalKeys["Left_000_1"] = "##_O_D";

    TerminalKeys["Up_CAS_0"] = "##_[_1_;_@_A";
    TerminalKeys["Down_CAS_0"] = "##_[_1_;_@_B";
    TerminalKeys["Right_CAS_0"] = "##_[_1_;_@_C";
    TerminalKeys["Left_CAS_0"] = "##_[_1_;_@_D";
    TerminalKeys["Up_CAS_1"] = "##_[_1_;_@_A";
    TerminalKeys["Down_CAS_1"] = "##_[_1_;_@_B";
    TerminalKeys["Right_CAS_1"] = "##_[_1_;_@_C";
    TerminalKeys["Left_CAS_1"] = "##_[_1_;_@_D";

    // Keys depending on configuration 1 and 2
    TerminalKeys["F1_000_0"] = "##_[_1_1_~";
    TerminalKeys["F2_000_0"] = "##_[_1_2_~";
    TerminalKeys["F3_000_0"] = "##_[_1_3_~";
    TerminalKeys["F4_000_0"] = "##_[_1_4_~";
    TerminalKeys["F5_000_0"] = "##_[_1_5_~";
    TerminalKeys["F6_000_0"] = "##_[_1_7_~";
    TerminalKeys["F7_000_0"] = "##_[_1_8_~";
    TerminalKeys["F8_000_0"] = "##_[_1_9_~";
    TerminalKeys["F9_000_0"] = "##_[_2_0_~";
    TerminalKeys["F10_000_0"] = "##_[_2_1_~";
    TerminalKeys["F11_000_0"] = "##_[_2_3_~";
    TerminalKeys["F12_000_0"] = "##_[_2_4_~";

    TerminalKeys["F13_000_0"] = "##_[_2_5_~";
    TerminalKeys["F14_000_0"] = "##_[_2_6_~";
    TerminalKeys["F15_000_0"] = "##_[_2_8_~";
    TerminalKeys["F16_000_0"] = "##_[_2_9_~";
    TerminalKeys["F17_000_0"] = "##_[_3_1_~";
    TerminalKeys["F18_000_0"] = "##_[_3_2_~";
    TerminalKeys["F19_000_0"] = "##_[_3_3_~";
    TerminalKeys["F20_000_0"] = "##_[_3_4_~";

    TerminalKeys["F1_CAS_0"] = "##_[_1_1_;_@_~";
    TerminalKeys["F2_CAS_0"] = "##_[_1_2_;_@_~";
    TerminalKeys["F3_CAS_0"] = "##_[_1_3_;_@_~";
    TerminalKeys["F4_CAS_0"] = "##_[_1_4_;_@_~";
    TerminalKeys["F5_CAS_0"] = "##_[_1_5_;_@_~";
    TerminalKeys["F6_CAS_0"] = "##_[_1_7_;_@_~";
    TerminalKeys["F7_CAS_0"] = "##_[_1_8_;_@_~";
    TerminalKeys["F8_CAS_0"] = "##_[_1_9_;_@_~";
    TerminalKeys["F9_CAS_0"] = "##_[_2_0_;_@_~";
    TerminalKeys["F10_CAS_0"] = "##_[_2_1_;_@_~";
    TerminalKeys["F11_CAS_0"] = "##_[_2_3_;_@_~";
    TerminalKeys["F12_CAS_0"] = "##_[_2_4_;_@_~";

    TerminalKeys["F13_CAS_0"] = "##_[_2_5_;_@_~";
    TerminalKeys["F14_CAS_0"] = "##_[_2_6_;_@_~";
    TerminalKeys["F15_CAS_0"] = "##_[_2_8_;_@_~";
    TerminalKeys["F16_CAS_0"] = "##_[_2_9_;_@_~";
    TerminalKeys["F17_CAS_0"] = "##_[_3_1_;_@_~";
    TerminalKeys["F18_CAS_0"] = "##_[_3_2_;_@_~";
    TerminalKeys["F19_CAS_0"] = "##_[_3_3_;_@_~";
    TerminalKeys["F20_CAS_0"] = "##_[_3_4_;_@_~";

    TerminalKeys["F1_000_1"] = "##_O_P";
    TerminalKeys["F2_000_1"] = "##_O_Q";
    TerminalKeys["F3_000_1"] = "##_O_R";
    TerminalKeys["F4_000_1"] = "##_O_S";
    TerminalKeys["F5_000_1"] = "##_O_T";
    TerminalKeys["F6_000_1"] = "##_O_U";
    TerminalKeys["F7_000_1"] = "##_O_V";
    TerminalKeys["F8_000_1"] = "##_O_W";
    TerminalKeys["F9_000_1"] = "##_O_X";
    TerminalKeys["F10_000_1"] = "##_O_Y";
    TerminalKeys["F11_000_1"] = "##_O_Z";
    TerminalKeys["F12_000_1"] = "##_O_[";

    TerminalKeys["F13_000_1"] = "##_O_\\";
    TerminalKeys["F14_000_1"] = "##_O_]";
    TerminalKeys["F15_000_1"] = "##_O_^";
    TerminalKeys["F16_000_1"] = "##_O__";
    TerminalKeys["F17_000_1"] = "";
    TerminalKeys["F18_000_1"] = "";
    TerminalKeys["F19_000_1"] = "";
    TerminalKeys["F20_000_1"] = "";

    TerminalKeys["F1_CAS_1"] = "##_[_1_;_@_P";
    TerminalKeys["F2_CAS_1"] = "##_[_1_;_@_Q";
    TerminalKeys["F3_CAS_1"] = "##_[_1_;_@_R";
    TerminalKeys["F4_CAS_1"] = "##_[_1_;_@_S";
    TerminalKeys["F5_CAS_1"] = "##_[_1_;_@_T";
    TerminalKeys["F6_CAS_1"] = "##_[_1_;_@_U";
    TerminalKeys["F7_CAS_1"] = "##_[_1_;_@_V";
    TerminalKeys["F8_CAS_1"] = "##_[_1_;_@_W";
    TerminalKeys["F9_CAS_1"] = "##_[_1_;_@_X";
    TerminalKeys["F10_CAS_1"] = "##_[_1_;_@_Y";
    TerminalKeys["F11_CAS_1"] = "##_[_1_;_@_Z";
    TerminalKeys["F12_CAS_1"] = "##_[_1_;_@_[";

    TerminalKeys["F13_CAS_1"] = "##_[_1_;_@_\\";
    TerminalKeys["F14_CAS_1"] = "##_[_1_;_@_]";
    TerminalKeys["F15_CAS_1"] = "##_[_1_;_@_^";
    TerminalKeys["F16_CAS_1"] = "##_[_1_;_@__";
    TerminalKeys["F17_CAS_1"] = "";
    TerminalKeys["F18_CAS_1"] = "";
    TerminalKeys["F19_CAS_1"] = "";
    TerminalKeys["F20_CAS_1"] = "";

    // Keys depending on configuration 3
    TerminalKeys["Insert_000_0"] = "##_[_2_~";
    TerminalKeys["Delete_000_0"] = "##_[_3_~";
    TerminalKeys["Home_000_0"] = "##_[_1_~";
    TerminalKeys["End_000_0"] = "##_[_4_~";
    TerminalKeys["PageUp_000_0"] = "##_[_5_~";
    TerminalKeys["PageDown_000_0"] = "##_[_6_~";
    TerminalKeys["Insert_000_1"] = "##_[_2_~";
    TerminalKeys["Delete_000_1"] = "##_[_3_~";
    TerminalKeys["Home_000_1"] = "##_[_H";
    TerminalKeys["End_000_1"] = "##_[_F";
    TerminalKeys["PageUp_000_1"] = "##_[_5_~";
    TerminalKeys["PageDown_000_1"] = "##_[_6_~";
    TerminalKeys["Insert_000_2"] = "##_[_2_~";
    TerminalKeys["Delete_000_2"] = "##_[_3_~";
    TerminalKeys["Home_000_2"] = "##_O_H";
    TerminalKeys["End_000_2"] = "##_O_F";
    TerminalKeys["PageUp_000_2"] = "##_[_5_~";
    TerminalKeys["PageDown_000_2"] = "##_[_6_~";

    TerminalKeys["Insert_CAS_0"] = "##_[_2_;_@_~";
    TerminalKeys["Delete_CAS_0"] = "##_[_3_;_@_~";
    TerminalKeys["Home_CAS_0"] = "##_[_1_;_@_~";
    TerminalKeys["End_CAS_0"] = "##_[_4_;_@_~";
    TerminalKeys["PageUp_CAS_0"] = "##_[_5_;_@_~";
    TerminalKeys["PageDown_CAS_0"] = "##_[_6_;_@_~";
    TerminalKeys["Insert_CAS_1"] = "##_[_2_;_@_~";
    TerminalKeys["Delete_CAS_1"] = "##_[_3_;_@_~";
    TerminalKeys["Home_CAS_1"] = "##_[_1_;_@_H";
    TerminalKeys["End_CAS_1"] = "##_[_1_;_@_F";
    TerminalKeys["PageUp_CAS_1"] = "##_[_5_;_@_~";
    TerminalKeys["PageDown_CAS_1"] = "##_[_6_;_@_~";
    TerminalKeys["Insert_CAS_2"] = "##_[_2_;_@_~";
    TerminalKeys["Delete_CAS_2"] = "##_[_3_;_@_~";
    TerminalKeys["Home_CAS_2"] = "##_[_1_;_@_H";
    TerminalKeys["End_CAS_2"] = "##_[_1_;_@_F";
    TerminalKeys["PageUp_CAS_2"] = "##_[_5_;_@_~";
    TerminalKeys["PageDown_CAS_2"] = "##_[_6_;_@_~";

    // Keys for VT52 mode
    TerminalKeys["Up_000_9"] = "##_A";
    TerminalKeys["Down_000_9"] = "##_B";
    TerminalKeys["Right_000_9"] = "##_C";
    TerminalKeys["Left_000_9"] = "##_D";
    TerminalKeys["F1_000_9"] = "##_P";
    TerminalKeys["F2_000_9"] = "##_Q";
    TerminalKeys["F3_000_9"] = "##_R";
    TerminalKeys["F4_000_9"] = "##_S";
    TerminalKeys["F5_000_9"] = "##_T";
    TerminalKeys["F6_000_9"] = "##_U";
    TerminalKeys["F7_000_9"] = "##_V";
    TerminalKeys["F8_000_9"] = "##_W";
    TerminalKeys["F9_000_9"] = "##_X";
    TerminalKeys["F10_000_9"] = "##_Y";
    TerminalKeys["F11_000_9"] = "##_Z";
    TerminalKeys["F12_000_9"] = "##_[";

    TerminalKeys["F13_000_9"] = "##_\\";
    TerminalKeys["F14_000_9"] = "##_]";
    TerminalKeys["F15_000_9"] = "##_^";
    TerminalKeys["F16_000_9"] = "##__";
    TerminalKeys["F17_000_9"] = "";
    TerminalKeys["F18_000_9"] = "";
    TerminalKeys["F19_000_9"] = "";
    TerminalKeys["F20_000_9"] = "";

    TerminalKeys["Insert_000_9"] = "##_L";
    TerminalKeys["Delete_000_9"] = "##_M";
    TerminalKeys["Home_000_9"] = "##_H";
    TerminalKeys["End_000_9"] = "##_E";
    TerminalKeys["PageUp_000_9"] = "##_I";
    TerminalKeys["PageDown_000_9"] = "##_G";

    // Alternate numeric keys
    TerminalKeys["NumPad_Numpad0_0"] = "##_O_p";
    TerminalKeys["NumPad_Numpad1_0"] = "##_O_q";
    TerminalKeys["NumPad_Numpad2_0"] = "##_O_r";
    TerminalKeys["NumPad_Numpad3_0"] = "##_O_s";
    TerminalKeys["NumPad_Numpad4_0"] = "##_O_t";
    TerminalKeys["NumPad_Numpad5_0"] = "##_O_u";
    TerminalKeys["NumPad_Numpad6_0"] = "##_O_v";
    TerminalKeys["NumPad_Numpad7_0"] = "##_O_w";
    TerminalKeys["NumPad_Numpad8_0"] = "##_O_x";
    TerminalKeys["NumPad_Numpad9_0"] = "##_O_y";
    TerminalKeys["NumPad_NumpadDecimal_0"] = "##_O_n";
    TerminalKeys["NumPad_NumpadAdd_0"] = "##_O_l";
    TerminalKeys["NumPad_NumpadSubtract_0"] = "##_O_m";
    TerminalKeys["NumPad_NumpadEnter_0"] = "##_O_M";
    TerminalKeys["NumPad_NumpadMultiply_0"] = "";
    TerminalKeys["NumPad_NumpadDivide_0"] = "";

    TerminalKeys["NumPad_Numpad0_1"] = "##_?_p";
    TerminalKeys["NumPad_Numpad1_1"] = "##_?_q";
    TerminalKeys["NumPad_Numpad2_1"] = "##_?_r";
    TerminalKeys["NumPad_Numpad3_1"] = "##_?_s";
    TerminalKeys["NumPad_Numpad4_1"] = "##_?_t";
    TerminalKeys["NumPad_Numpad5_1"] = "##_?_u";
    TerminalKeys["NumPad_Numpad6_1"] = "##_?_v";
    TerminalKeys["NumPad_Numpad7_1"] = "##_?_w";
    TerminalKeys["NumPad_Numpad8_1"] = "##_?_x";
    TerminalKeys["NumPad_Numpad9_1"] = "##_?_y";
    TerminalKeys["NumPad_NumpadDecimal_1"] = "##_?_n";
    TerminalKeys["NumPad_NumpadAdd_1"] = "##_?_l";
    TerminalKeys["NumPad_NumpadSubtract_1"] = "##_?_m";
    TerminalKeys["NumPad_NumpadEnter_1"] = "##_?_M";
    TerminalKeys["NumPad_NumpadMultiply_1"] = "";
    TerminalKeys["NumPad_NumpadDivide_1"] = "";

    // Generating codes for CAS modifiers
    XList<std::string> TerminalKeysCAS;
    TerminalKeysCAS.AddRange(TerminalKeys0);
    TerminalKeysCAS.AddRange(TerminalKeys1);
    TerminalKeysCAS.AddRange(TerminalKeys2);
    TerminalKeysCAS.AddRange(TerminalKeys3);
    TerminalKeysCAS.AddRange(TerminalKeys4);
    for (int i = 0; i < TerminalKeysCAS.Count; i++)
    {
        std::string N = TerminalKeysCAS[i];
        for (int ii = 0; ii < 3; ii++)
        {
            std::string N_ = std::to_string(ii);
            if (TerminalKeys.count(N + "_000_" + N_) > 0)
            {
                std::string NN = TerminalKeys[N + "_CAS_" + N_];
                TerminalKeys[N + "_001_" + N_] = TextWork::StringReplace(NN, "_@", "_2");
                TerminalKeys[N + "_010_" + N_] = TextWork::StringReplace(NN, "_@", "_3");
                TerminalKeys[N + "_011_" + N_] = TextWork::StringReplace(NN, "_@", "_4");
                TerminalKeys[N + "_100_" + N_] = TextWork::StringReplace(NN, "_@", "_5");
                TerminalKeys[N + "_101_" + N_] = TextWork::StringReplace(NN, "_@", "_6");
                TerminalKeys[N + "_110_" + N_] = TextWork::StringReplace(NN, "_@", "_7");
                TerminalKeys[N + "_111_" + N_] = TextWork::StringReplace(NN, "_@", "_8");
            }
        }
        if (TerminalKeys.count(N + "_000_9") > 0)
        {
            TerminalKeys[N + "_001_9"] = TerminalKeys[N + "_000_9"];
            TerminalKeys[N + "_010_9"] = TerminalKeys[N + "_000_9"];
            TerminalKeys[N + "_011_9"] = TerminalKeys[N + "_000_9"];
            TerminalKeys[N + "_100_9"] = TerminalKeys[N + "_000_9"];
            TerminalKeys[N + "_101_9"] = TerminalKeys[N + "_000_9"];
            TerminalKeys[N + "_110_9"] = TerminalKeys[N + "_000_9"];
            TerminalKeys[N + "_111_9"] = TerminalKeys[N + "_000_9"];
        }
    }
}

std::string TerminalKeyboard::GetTelnetKeyboardConf(int N)
{
    if (CoreAnsi_.get()->AnsiState_.__AnsiVT52 && (N <= 3))
    {
        return "9";
    }
    return TextWork::CharToStr(TelnetKeyboardConf[N]);
}

void TerminalKeyboard::SetTelnetKeyboardConf(int N, int V)
{
    std::string S1 = "";
    std::string S2 = "";
    if (N > 0)
    {
        S1 = TelnetKeyboardConf.substr(0, N);
    }
    if (N < (TelnetKeyboardConfMax.size() - 1))
    {
        S2 = TelnetKeyboardConf.substr(N + 1);
    }
    TelnetKeyboardConf = (S1 + std::to_string(V) + S2);
}

std::string TerminalKeyboard::KeyCAS(bool ModShift, bool ModCtrl, bool ModAlt)
{
    std::string CAS = "";
    if (ModCtrl || (TelnetKeyboardMods[0] != '0')) { CAS = CAS + "1"; } else { CAS = CAS + "0"; }
    if (ModAlt || (TelnetKeyboardMods[1] != '0')) { CAS = CAS + "1"; } else { CAS = CAS + "0"; }
    if (ModShift || (TelnetKeyboardMods[2] != '0')) { CAS = CAS + "1"; } else { CAS = CAS + "0"; }
    return CAS;
}

std::string TerminalKeyboard::KeyCode(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    EchoChar.Clear();
    std::string CAS = KeyCAS(ModShift, ModCtrl, ModAlt);
    bool FuncShift = (TelnetFuncKeyOther == 1);
    switch (_(KeyName.c_str()))
    {
        case _("ArrowUp"):     return TerminalKeys["Up_" + CAS + "_" + GetTelnetKeyboardConf(0)];
        case _("ArrowDown"):   return TerminalKeys["Down_" + CAS + "_" + GetTelnetKeyboardConf(0)];
        case _("ArrowLeft"):   return TerminalKeys["Left_" + CAS + "_" + GetTelnetKeyboardConf(0)];
        case _("ArrowRight"):  return TerminalKeys["Right_" + CAS + "_" + GetTelnetKeyboardConf(0)];
        case _("Insert"):      return TerminalKeys["Insert_" + CAS + "_" + GetTelnetKeyboardConf(3)];
        case _("Delete"):      return TerminalKeys["Delete_" + CAS + "_" + GetTelnetKeyboardConf(3)];
        case _("Home"):        return TerminalKeys["Home_" + CAS + "_" + GetTelnetKeyboardConf(3)];
        case _("End"):         return TerminalKeys["End_" + CAS + "_" + GetTelnetKeyboardConf(3)];
        case _("PageUp"):      return TerminalKeys["PageUp_" + CAS + "_" + GetTelnetKeyboardConf(3)];
        case _("PageDown"):    return TerminalKeys["PageDown_" + CAS + "_" + GetTelnetKeyboardConf(3)];
        case _("Escape"):      return TerminalKeys["Escape"];
        case _("Tab"):         return TerminalKeys["Tab"];

        case _("Backspace"):
            EchoChar.Add(8);
            return TerminalKeys["Backspace_" + GetTelnetKeyboardConf(5)];
        case _("Enter"):
            EchoChar.Add(13);
            EchoChar.Add(10);
            return TerminalKeys["Enter_" + GetTelnetKeyboardConf(4)];
        case _("F1"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F11_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F1_" + CAS + "_" + GetTelnetKeyboardConf(1)]; }
            }
            else
            {
                return "00";
            }
        case _("F2"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F12_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F2_" + CAS + "_" + GetTelnetKeyboardConf(1)]; }
            }
            else
            {
                return "1B";
            }
        case _("F3"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F13_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F3_" + CAS + "_" + GetTelnetKeyboardConf(1)]; }
            }
            else
            {
                return "1C";
            }
        case _("F4"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F14_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F4_" + CAS + "_" + GetTelnetKeyboardConf(1)]; }
            }
            else
            {
                return "1D";
            }
        case _("F5"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F15_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F5_" + CAS + "_" + GetTelnetKeyboardConf(2)]; }
            }
            else
            {
                return "1E";
            }
        case _("F6"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F16_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F6_" + CAS + "_" + GetTelnetKeyboardConf(2)]; }
            }
            else
            {
                return "1F";
            }
        case _("F7"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F17_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F7_" + CAS + "_" + GetTelnetKeyboardConf(2)]; }
            }
            else
            {
                return "7F";
            }
        case _("F8"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F18_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F8_" + CAS + "_" + GetTelnetKeyboardConf(2)]; }
            }
            else
            {
                return "";
            }
        case _("F9"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F19_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F9_" + CAS + "_" + GetTelnetKeyboardConf(2)]; }
            }
            else
            {
                return "";
            }
        case _("F10"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return TerminalKeys["F20_" + CAS + "_" + GetTelnetKeyboardConf(2)]; } else { return TerminalKeys["F10_" + CAS + "_" + GetTelnetKeyboardConf(2)]; }
            }
            else
            {
                return "";
            }
        case _("F11"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return ""; } else { return TerminalKeys["F11_" + CAS + "_" + GetTelnetKeyboardConf(2)]; }
            }
            else
            {
                return TerminalAnswerBack;
            }
        case _("F12"):
            if (TelnetFuncKeyOther < 2)
            {
                if (FuncShift) { return ""; } else { return TerminalKeys["F12_" + CAS + "_" + GetTelnetKeyboardConf(2)]; }
            }
            else
            {
                return TerminalAnswerBack;
            }

        case _("F13"): return TerminalKeys["F11_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F14"): return TerminalKeys["F12_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F15"): return TerminalKeys["F13_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F16"): return TerminalKeys["F14_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F17"): return TerminalKeys["F15_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F18"): return TerminalKeys["F16_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F19"): return TerminalKeys["F17_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F20"): return TerminalKeys["F18_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F21"): return TerminalKeys["F19_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F22"): return TerminalKeys["F20_" + CAS + "_" + GetTelnetKeyboardConf(2)];
        case _("F23"): return "";
        case _("F24"): return "";

        default:
            if (ModCtrl)
            {
                switch (KeyChar)
                {
                    case '2': case ' ': case '@': return "00";
                    case 'a': case 'A':           return "01";
                    case 'b': case 'B':           return "02";
                    case 'c': case 'C':           return "03";
                    case 'd': case 'D':           return "04";
                    case 'e': case 'E':           return "05";
                    case 'f': case 'F':           return "06";
                    case 'g': case 'G':           return "07";
                    case 'h': case 'H':           return "08";
                    case 'i': case 'I':           return "09";
                    case 'j': case 'J':           return "0A";
                    case 'k': case 'K':           return "0B";
                    case 'l': case 'L':           return "0C";
                    case 'm': case 'M':           return "0D";
                    case 'n': case 'N':           return "0E";
                    case 'o': case 'O':           return "0F";
                    case 'p': case 'P':           return "10";
                    case 'q': case 'Q':           return "11";
                    case 'r': case 'R':           return "12";
                    case 's': case 'S':           return "13";
                    case 't': case 'T':           return "14";
                    case 'u': case 'U':           return "15";
                    case 'v': case 'V':           return "16";
                    case 'w': case 'W':           return "17";
                    case 'x': case 'X':           return "18";
                    case 'y': case 'Y':           return "19";
                    case 'z': case 'Z':           return "1A";
                    case '3': case '[': case '{': return "1B";
                    case '4': case '\\':case '|': return "1C";
                    case '5': case ']': case '}': return "1D";
                    case '6': case '~': case '^': return "1E";
                    case '7': case '/': case '_': return "1F";
                    case '8': case '?':           return "7F";
                    default:                      return "";
                }
            }
            else
            {
                bool StdKey = true;
                if (GetTelnetKeyboardConf(6) == "1")
                {
                    switch (_(KeyName.c_str()))
                    {
                        case _("Numpad0"):
                        case _("Numpad1"):
                        case _("Numpad2"):
                        case _("Numpad3"):
                        case _("Numpad4"):
                        case _("Numpad5"):
                        case _("Numpad6"):
                        case _("Numpad7"):
                        case _("Numpad8"):
                        case _("Numpad9"):
                        case _("NumpadDecimal"):
                        case _("NumpadAdd"):
                        case _("NumpadSubtract"):
                        case _("NumpadEnter"):
                        case _("NumpadMultiply"):
                        case _("NumpadDivide"):
                            if (CoreAnsi_.get()->AnsiState_.__AnsiVT52)
                            {
                                return TerminalKeys["NumPad_" + KeyName + "_1"];
                            }
                            else
                            {
                                return TerminalKeys["NumPad_" + KeyName + "_0"];
                            }
                    }
                }
                if (StdKey)
                {
                    if ((KeyChar >= 32) || ((KeyChar >= 1) && (KeyChar <= 26) && (KeyChar != 13)))
                    {
                        EchoChar.Add(KeyChar);
                        return "~~" + Hex::IntToHex32(KeyChar);
                    }
                    if (KeyName == "NumpadEnter")
                    {
                        EchoChar.Add(13);
                        EchoChar.Add(10);
                        return TerminalKeys["Enter_" + GetTelnetKeyboardConf(4)];
                    }
                }
                return "";
            }
    }
}

void TerminalKeyboard::TelnetKeyboardConfMove()
{
    TelnetKeyboardConfI++;
    if (TelnetKeyboardConfI == TelnetKeyboardConfMax.size())
    {
        TelnetKeyboardConfI = 0;
    }
}

void TerminalKeyboard::TelnetKeyboardConfStep()
{
    int Val = std::stoi(TextWork::CharToStr(TelnetKeyboardConf[TelnetKeyboardConfI]));

    if (TelnetKeyboardConf[TelnetKeyboardConfI] == TelnetKeyboardConfMax[TelnetKeyboardConfI])
    {
        Val = 0;
    }
    else
    {
        Val++;
    }
    std::string X = "";
    for (int i = 0; i < TelnetKeyboardConf.size(); i++)
    {
        if (i == TelnetKeyboardConfI)
        {
            X = X + std::to_string(Val);
        }
        else
        {
            X = X + TextWork::CharToStr(TelnetKeyboardConf[i]);
        }
    }
    TelnetKeyboardConf = X;
}

void TerminalKeyboard::TelnetKeyboardModsMove()
{
    TelnetKeyboardModsI++;
    if (TelnetKeyboardModsI == TelnetKeyboardMods.size())
    {
        TelnetKeyboardModsI = 0;
    }
}

void TerminalKeyboard::TelnetKeyboardModsStep()
{
    std::string Val = std::to_string(TelnetKeyboardMods[TelnetKeyboardModsI]);

    if (TelnetKeyboardMods[TelnetKeyboardModsI] == '1')
    {
        Val = "0";
    }
    else
    {
        Val = "1";
    }
    std::string X = "";
    for (int i = 0; i < TelnetKeyboardMods.size(); i++)
    {
        if (i == TelnetKeyboardModsI)
        {
            X = X + Val;
        }
        else
        {
            X = X + TextWork::CharToStr(TelnetKeyboardMods[i]);
        }
    }
    TelnetKeyboardMods = X;
}
