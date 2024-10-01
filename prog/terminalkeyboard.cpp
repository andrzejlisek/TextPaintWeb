#include "terminalkeyboard.h"

TerminalKeyboard::TerminalKeyboard()
{
    std::vector<std::string> TerminalKeysCAS;
    TerminalKeysCAS.push_back("Up");
    TerminalKeysCAS.push_back("Down");
    TerminalKeysCAS.push_back("Right");
    TerminalKeysCAS.push_back("Left");
    TerminalKeysCAS.push_back("F1");
    TerminalKeysCAS.push_back("F2");
    TerminalKeysCAS.push_back("F3");
    TerminalKeysCAS.push_back("F4");
    TerminalKeysCAS.push_back("F5");
    TerminalKeysCAS.push_back("F6");
    TerminalKeysCAS.push_back("F7");
    TerminalKeysCAS.push_back("F8");
    TerminalKeysCAS.push_back("F9");
    TerminalKeysCAS.push_back("F10");
    TerminalKeysCAS.push_back("F11");
    TerminalKeysCAS.push_back("F12");
    TerminalKeysCAS.push_back("Home");
    TerminalKeysCAS.push_back("End");
    TerminalKeysCAS.push_back("Insert");
    TerminalKeysCAS.push_back("Delete");
    TerminalKeysCAS.push_back("PageUp");
    TerminalKeysCAS.push_back("PageDown");
    TerminalKeysCAS.push_back("F13");
    TerminalKeysCAS.push_back("F14");
    TerminalKeysCAS.push_back("F15");
    TerminalKeysCAS.push_back("F16");
    TerminalKeysCAS.push_back("F17");
    TerminalKeysCAS.push_back("F18");
    TerminalKeysCAS.push_back("F19");
    TerminalKeysCAS.push_back("F20");

    std::unordered_map<std::string, std::string> TerminalKeysTempl;

    // Keys depending on configuration 0
    TerminalKeysTempl["Up_CAS_0"] = "##_[_1_;_@_A";
    TerminalKeysTempl["Down_CAS_0"] = "##_[_1_;_@_B";
    TerminalKeysTempl["Right_CAS_0"] = "##_[_1_;_@_C";
    TerminalKeysTempl["Left_CAS_0"] = "##_[_1_;_@_D";
    TerminalKeysTempl["Up_CAS_1"] = "##_[_1_;_@_A";
    TerminalKeysTempl["Down_CAS_1"] = "##_[_1_;_@_B";
    TerminalKeysTempl["Right_CAS_1"] = "##_[_1_;_@_C";
    TerminalKeysTempl["Left_CAS_1"] = "##_[_1_;_@_D";

    // Keys depending on configuration 1 and 2
    TerminalKeysTempl["F1_CAS_0"] = "##_[_1_1_;_@_~";
    TerminalKeysTempl["F2_CAS_0"] = "##_[_1_2_;_@_~";
    TerminalKeysTempl["F3_CAS_0"] = "##_[_1_3_;_@_~";
    TerminalKeysTempl["F4_CAS_0"] = "##_[_1_4_;_@_~";
    TerminalKeysTempl["F5_CAS_0"] = "##_[_1_5_;_@_~";
    TerminalKeysTempl["F6_CAS_0"] = "##_[_1_7_;_@_~";
    TerminalKeysTempl["F7_CAS_0"] = "##_[_1_8_;_@_~";
    TerminalKeysTempl["F8_CAS_0"] = "##_[_1_9_;_@_~";
    TerminalKeysTempl["F9_CAS_0"] = "##_[_2_0_;_@_~";
    TerminalKeysTempl["F10_CAS_0"] = "##_[_2_1_;_@_~";
    TerminalKeysTempl["F11_CAS_0"] = "##_[_2_3_;_@_~";
    TerminalKeysTempl["F12_CAS_0"] = "##_[_2_4_;_@_~";

    TerminalKeysTempl["F13_CAS_0"] = "##_[_2_5_;_@_~";
    TerminalKeysTempl["F14_CAS_0"] = "##_[_2_6_;_@_~";
    TerminalKeysTempl["F15_CAS_0"] = "##_[_2_8_;_@_~";
    TerminalKeysTempl["F16_CAS_0"] = "##_[_2_9_;_@_~";
    TerminalKeysTempl["F17_CAS_0"] = "##_[_3_1_;_@_~";
    TerminalKeysTempl["F18_CAS_0"] = "##_[_3_2_;_@_~";
    TerminalKeysTempl["F19_CAS_0"] = "##_[_3_3_;_@_~";
    TerminalKeysTempl["F20_CAS_0"] = "##_[_3_4_;_@_~";

    TerminalKeysTempl["F1_CAS_1"] = "##_[_1_;_@_P";
    TerminalKeysTempl["F2_CAS_1"] = "##_[_1_;_@_Q";
    TerminalKeysTempl["F3_CAS_1"] = "##_[_1_;_@_R";
    TerminalKeysTempl["F4_CAS_1"] = "##_[_1_;_@_S";
    TerminalKeysTempl["F5_CAS_1"] = "##_[_1_;_@_T";
    TerminalKeysTempl["F6_CAS_1"] = "##_[_1_;_@_U";
    TerminalKeysTempl["F7_CAS_1"] = "##_[_1_;_@_V";
    TerminalKeysTempl["F8_CAS_1"] = "##_[_1_;_@_W";
    TerminalKeysTempl["F9_CAS_1"] = "##_[_1_;_@_X";
    TerminalKeysTempl["F10_CAS_1"] = "##_[_1_;_@_Y";
    TerminalKeysTempl["F11_CAS_1"] = "##_[_1_;_@_Z";
    TerminalKeysTempl["F12_CAS_1"] = "##_[_1_;_@_[";

    TerminalKeysTempl["F13_CAS_1"] = "##_[_1_;_@_\\";
    TerminalKeysTempl["F14_CAS_1"] = "##_[_1_;_@_]";
    TerminalKeysTempl["F15_CAS_1"] = "##_[_1_;_@_^";
    TerminalKeysTempl["F16_CAS_1"] = "##_[_1_;_@__";
    TerminalKeysTempl["F17_CAS_1"] = "";
    TerminalKeysTempl["F18_CAS_1"] = "";
    TerminalKeysTempl["F19_CAS_1"] = "";
    TerminalKeysTempl["F20_CAS_1"] = "";

    // Keys depending on configuration 3
    TerminalKeysTempl["Insert_CAS_0"] = "##_[_2_;_@_~";
    TerminalKeysTempl["Delete_CAS_0"] = "##_[_3_;_@_~";
    TerminalKeysTempl["Home_CAS_0"] = "##_[_1_;_@_~";
    TerminalKeysTempl["End_CAS_0"] = "##_[_4_;_@_~";
    TerminalKeysTempl["PageUp_CAS_0"] = "##_[_5_;_@_~";
    TerminalKeysTempl["PageDown_CAS_0"] = "##_[_6_;_@_~";
    TerminalKeysTempl["Insert_CAS_1"] = "##_[_2_;_@_~";
    TerminalKeysTempl["Delete_CAS_1"] = "##_[_3_;_@_~";
    TerminalKeysTempl["Home_CAS_1"] = "##_[_1_;_@_H";
    TerminalKeysTempl["End_CAS_1"] = "##_[_1_;_@_F";
    TerminalKeysTempl["PageUp_CAS_1"] = "##_[_5_;_@_~";
    TerminalKeysTempl["PageDown_CAS_1"] = "##_[_6_;_@_~";
    TerminalKeysTempl["Insert_CAS_2"] = "##_[_2_;_@_~";
    TerminalKeysTempl["Delete_CAS_2"] = "##_[_3_;_@_~";
    TerminalKeysTempl["Home_CAS_2"] = "##_[_1_;_@_H";
    TerminalKeysTempl["End_CAS_2"] = "##_[_1_;_@_F";
    TerminalKeysTempl["PageUp_CAS_2"] = "##_[_5_;_@_~";
    TerminalKeysTempl["PageDown_CAS_2"] = "##_[_6_;_@_~";

    // Keys for VT52 mode
    TerminalKeysTempl["Up_000_9"] = "##_A";
    TerminalKeysTempl["Down_000_9"] = "##_B";
    TerminalKeysTempl["Right_000_9"] = "##_C";
    TerminalKeysTempl["Left_000_9"] = "##_D";
    TerminalKeysTempl["F1_000_9"] = "##_P";
    TerminalKeysTempl["F2_000_9"] = "##_Q";
    TerminalKeysTempl["F3_000_9"] = "##_R";
    TerminalKeysTempl["F4_000_9"] = "##_S";
    TerminalKeysTempl["F5_000_9"] = "##_T";
    TerminalKeysTempl["F6_000_9"] = "##_U";
    TerminalKeysTempl["F7_000_9"] = "##_V";
    TerminalKeysTempl["F8_000_9"] = "##_W";
    TerminalKeysTempl["F9_000_9"] = "##_X";
    TerminalKeysTempl["F10_000_9"] = "##_Y";
    TerminalKeysTempl["F11_000_9"] = "##_Z";
    TerminalKeysTempl["F12_000_9"] = "##_[";

    TerminalKeysTempl["F13_000_9"] = "##_\\";
    TerminalKeysTempl["F14_000_9"] = "##_]";
    TerminalKeysTempl["F15_000_9"] = "##_^";
    TerminalKeysTempl["F16_000_9"] = "##__";
    TerminalKeysTempl["F17_000_9"] = "";
    TerminalKeysTempl["F18_000_9"] = "";
    TerminalKeysTempl["F19_000_9"] = "";
    TerminalKeysTempl["F20_000_9"] = "";

    TerminalKeysTempl["Insert_000_9"] = "##_L";
    TerminalKeysTempl["Delete_000_9"] = "##_M";
    TerminalKeysTempl["Home_000_9"] = "##_H";
    TerminalKeysTempl["End_000_9"] = "##_E";
    TerminalKeysTempl["PageUp_000_9"] = "##_I";
    TerminalKeysTempl["PageDown_000_9"] = "##_G";

    std::unordered_map<std::string, std::string> TerminalKeys_;

    // Generating codes for CAS modifiers
    for (int i = 0; i < TerminalKeysCAS.size(); i++)
    {
        std::string N = TerminalKeysCAS[i];
        for (int ii = 0; ii < 3; ii++)
        {
            std::string N_ = std::to_string(ii);
            if (TerminalKeysTempl.count(N + "_000_" + N_) > 0)
            {
                std::string NN = TerminalKeysTempl[N + "_CAS_" + N_];
                TerminalKeys_[N + "_001_" + N_] = TextWork::StringReplace(NN, "_@", "_2");
                TerminalKeys_[N + "_010_" + N_] = TextWork::StringReplace(NN, "_@", "_3");
                TerminalKeys_[N + "_011_" + N_] = TextWork::StringReplace(NN, "_@", "_4");
                TerminalKeys_[N + "_100_" + N_] = TextWork::StringReplace(NN, "_@", "_5");
                TerminalKeys_[N + "_101_" + N_] = TextWork::StringReplace(NN, "_@", "_6");
                TerminalKeys_[N + "_110_" + N_] = TextWork::StringReplace(NN, "_@", "_7");
                TerminalKeys_[N + "_111_" + N_] = TextWork::StringReplace(NN, "_@", "_8");
            }
        }
        if (TerminalKeysTempl.count(N + "_000_9") > 0)
        {
            TerminalKeys_[N + "_000_9"] = TerminalKeysTempl[N + "_000_9"];
            TerminalKeys_[N + "_001_9"] = TerminalKeysTempl[N + "_000_9"];
            TerminalKeys_[N + "_010_9"] = TerminalKeysTempl[N + "_000_9"];
            TerminalKeys_[N + "_011_9"] = TerminalKeysTempl[N + "_000_9"];
            TerminalKeys_[N + "_100_9"] = TerminalKeysTempl[N + "_000_9"];
            TerminalKeys_[N + "_101_9"] = TerminalKeysTempl[N + "_000_9"];
            TerminalKeys_[N + "_110_9"] = TerminalKeysTempl[N + "_000_9"];
            TerminalKeys_[N + "_111_9"] = TerminalKeysTempl[N + "_000_9"];
        }
    }

    // Common keys
    TerminalKeys_["Escape"] = "1B";
    TerminalKeys_["Tab"] = "09";
    TerminalKeys_["Enter_0"] = "0D";
    TerminalKeys_["Enter_1"] = "0D0A";
    TerminalKeys_["Enter_2"] = "0A";
    TerminalKeys_["Enter_3"] = "0D0A";
    TerminalKeys_["Backspace_0"] = "7F";
    TerminalKeys_["Backspace_1"] = "08";

    // Keys depending on configuration 0
    TerminalKeys_["Up_000_0"] = "##_[_A";
    TerminalKeys_["Down_000_0"] = "##_[_B";
    TerminalKeys_["Right_000_0"] = "##_[_C";
    TerminalKeys_["Left_000_0"] = "##_[_D";
    TerminalKeys_["Up_000_1"] = "##_O_A";
    TerminalKeys_["Down_000_1"] = "##_O_B";
    TerminalKeys_["Right_000_1"] = "##_O_C";
    TerminalKeys_["Left_000_1"] = "##_O_D";

    // Keys depending on configuration 1 and 2
    TerminalKeys_["F1_000_0"] = "##_[_1_1_~";
    TerminalKeys_["F2_000_0"] = "##_[_1_2_~";
    TerminalKeys_["F3_000_0"] = "##_[_1_3_~";
    TerminalKeys_["F4_000_0"] = "##_[_1_4_~";
    TerminalKeys_["F5_000_0"] = "##_[_1_5_~";
    TerminalKeys_["F6_000_0"] = "##_[_1_7_~";
    TerminalKeys_["F7_000_0"] = "##_[_1_8_~";
    TerminalKeys_["F8_000_0"] = "##_[_1_9_~";
    TerminalKeys_["F9_000_0"] = "##_[_2_0_~";
    TerminalKeys_["F10_000_0"] = "##_[_2_1_~";
    TerminalKeys_["F11_000_0"] = "##_[_2_3_~";
    TerminalKeys_["F12_000_0"] = "##_[_2_4_~";

    TerminalKeys_["F13_000_0"] = "##_[_2_5_~";
    TerminalKeys_["F14_000_0"] = "##_[_2_6_~";
    TerminalKeys_["F15_000_0"] = "##_[_2_8_~";
    TerminalKeys_["F16_000_0"] = "##_[_2_9_~";
    TerminalKeys_["F17_000_0"] = "##_[_3_1_~";
    TerminalKeys_["F18_000_0"] = "##_[_3_2_~";
    TerminalKeys_["F19_000_0"] = "##_[_3_3_~";
    TerminalKeys_["F20_000_0"] = "##_[_3_4_~";

    TerminalKeys_["F1_000_1"] = "##_O_P";
    TerminalKeys_["F2_000_1"] = "##_O_Q";
    TerminalKeys_["F3_000_1"] = "##_O_R";
    TerminalKeys_["F4_000_1"] = "##_O_S";
    TerminalKeys_["F5_000_1"] = "##_O_T";
    TerminalKeys_["F6_000_1"] = "##_O_U";
    TerminalKeys_["F7_000_1"] = "##_O_V";
    TerminalKeys_["F8_000_1"] = "##_O_W";
    TerminalKeys_["F9_000_1"] = "##_O_X";
    TerminalKeys_["F10_000_1"] = "##_O_Y";
    TerminalKeys_["F11_000_1"] = "##_O_Z";
    TerminalKeys_["F12_000_1"] = "##_O_[";

    TerminalKeys_["F13_000_1"] = "##_O_\\";
    TerminalKeys_["F14_000_1"] = "##_O_]";
    TerminalKeys_["F15_000_1"] = "##_O_^";
    TerminalKeys_["F16_000_1"] = "##_O__";
    TerminalKeys_["F17_000_1"] = "";
    TerminalKeys_["F18_000_1"] = "";
    TerminalKeys_["F19_000_1"] = "";
    TerminalKeys_["F20_000_1"] = "";

    // Keys depending on configuration 3
    TerminalKeys_["Insert_000_0"] = "##_[_2_~";
    TerminalKeys_["Delete_000_0"] = "##_[_3_~";
    TerminalKeys_["Home_000_0"] = "##_[_1_~";
    TerminalKeys_["End_000_0"] = "##_[_4_~";
    TerminalKeys_["PageUp_000_0"] = "##_[_5_~";
    TerminalKeys_["PageDown_000_0"] = "##_[_6_~";
    TerminalKeys_["Insert_000_1"] = "##_[_2_~";
    TerminalKeys_["Delete_000_1"] = "##_[_3_~";
    TerminalKeys_["Home_000_1"] = "##_[_H";
    TerminalKeys_["End_000_1"] = "##_[_F";
    TerminalKeys_["PageUp_000_1"] = "##_[_5_~";
    TerminalKeys_["PageDown_000_1"] = "##_[_6_~";
    TerminalKeys_["Insert_000_2"] = "##_[_2_~";
    TerminalKeys_["Delete_000_2"] = "##_[_3_~";
    TerminalKeys_["Home_000_2"] = "##_O_H";
    TerminalKeys_["End_000_2"] = "##_O_F";
    TerminalKeys_["PageUp_000_2"] = "##_[_5_~";
    TerminalKeys_["PageDown_000_2"] = "##_[_6_~";

    // Alternate numeric keys
    TerminalKeys_["NumPad_Numpad0_0"] = "##_O_p";
    TerminalKeys_["NumPad_Numpad1_0"] = "##_O_q";
    TerminalKeys_["NumPad_Numpad2_0"] = "##_O_r";
    TerminalKeys_["NumPad_Numpad3_0"] = "##_O_s";
    TerminalKeys_["NumPad_Numpad4_0"] = "##_O_t";
    TerminalKeys_["NumPad_Numpad5_0"] = "##_O_u";
    TerminalKeys_["NumPad_Numpad6_0"] = "##_O_v";
    TerminalKeys_["NumPad_Numpad7_0"] = "##_O_w";
    TerminalKeys_["NumPad_Numpad8_0"] = "##_O_x";
    TerminalKeys_["NumPad_Numpad9_0"] = "##_O_y";
    TerminalKeys_["NumPad_NumpadDecimal_0"] = "##_O_n";
    TerminalKeys_["NumPad_NumpadAdd_0"] = "##_O_l";
    TerminalKeys_["NumPad_NumpadSubtract_0"] = "##_O_m";
    TerminalKeys_["NumPad_NumpadEnter_0"] = "##_O_M";
    TerminalKeys_["NumPad_NumpadMultiply_0"] = "";
    TerminalKeys_["NumPad_NumpadDivide_0"] = "";

    TerminalKeys_["NumPad_Numpad0_1"] = "##_?_p";
    TerminalKeys_["NumPad_Numpad1_1"] = "##_?_q";
    TerminalKeys_["NumPad_Numpad2_1"] = "##_?_r";
    TerminalKeys_["NumPad_Numpad3_1"] = "##_?_s";
    TerminalKeys_["NumPad_Numpad4_1"] = "##_?_t";
    TerminalKeys_["NumPad_Numpad5_1"] = "##_?_u";
    TerminalKeys_["NumPad_Numpad6_1"] = "##_?_v";
    TerminalKeys_["NumPad_Numpad7_1"] = "##_?_w";
    TerminalKeys_["NumPad_Numpad8_1"] = "##_?_x";
    TerminalKeys_["NumPad_Numpad9_1"] = "##_?_y";
    TerminalKeys_["NumPad_NumpadDecimal_1"] = "##_?_n";
    TerminalKeys_["NumPad_NumpadAdd_1"] = "##_?_l";
    TerminalKeys_["NumPad_NumpadSubtract_1"] = "##_?_m";
    TerminalKeys_["NumPad_NumpadEnter_1"] = "##_?_M";
    TerminalKeys_["NumPad_NumpadMultiply_1"] = "";
    TerminalKeys_["NumPad_NumpadDivide_1"] = "";

    // Woraround to fix -O3 compilation bug
    //if (Screen::CurrentOpt < 0)
    //{
    //    //std::cout << "{" << TerminalKeys_["Right_000_1"] << "}" << std::endl;
    //}


    for (auto item : TerminalKeys_)
    {
        TerminalKeys[item.first] = item.second;

        bool IsError = false;
        std::string KeyStrTest = item.second;
        if ((KeyStrTest.length() % 2) != 0)
        {
            IsError = true;
        }
        for (int I = 0; I < KeyStrTest.length(); I++)
        {
            if ((KeyStrTest[I] < 32) || (KeyStrTest[I] > 126))
            {
                IsError = true;
            }
        }
        if (IsError)
        {
            std::cout << " [" << item.first << "]  [" << item.second << "]" << std::endl;
            //std::cout << "ERROR: [" << item.first << "]  [" << item.second << "]" << std::endl;
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
