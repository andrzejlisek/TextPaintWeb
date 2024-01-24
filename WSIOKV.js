class WebSocketInputOutputKeyValue
{
    constructor()
    {
        this.Raw = {};
        this.CaseMode = 0;
    }
    
    Case(S)
    {
        if (this.CaseMode > 0)
        {
            return S.toUpperCase();
        }
        if (this.CaseMode < 0)
        {
            return S.toLowerCase();
        }
        return S;
    }
            
    StringLoad(Data)
    {
        this.ParamClear();
        let Data_ = Data.split('\n');
        for (let II = 0; II < Data_.length; II++)
        {
            let S = Data_[II];
            let I = S.indexOf('=');
            if (I >= 0)
            {
                let RawK = this.Case(S.substr(0, I));
                if (!this.Raw[RawK])
                {
                    if (S.length > (I + 1))
                    {
                        this.Raw[RawK] = S.substr(I + 1);
                    }
                    else
                    {
                        this.Raw[RawK] = "";
                    }
                }
            }
        }
    }
    
    StringSave(Data)
    {
        let SB = "";
        for (var item in this.Raw)
        {
            SB = SB + this.Case(item) + "=" + this.Raw[item] + "\n";
        }
        return SB;
    }
    
    Print()
    {
        let SB = "";
        for (var item in this.Raw)
        {
            SB = SB + this.Case(item) + "=" + this.Raw[item] + "\n";
        }
        return SB;
    }
    
    ParamClear()
    {
        this.Raw = {};
    }
    
    ParamRemove(Name)
    {
        if (this.Raw[this.Case(Name)])
        {
            delete this.Raw[this.Case(Name)];
        }
    }
    
    ParamSet(Name, Value)
    {
        if (typeof Value == "boolean")
        {
            this.Raw[Name] = Value ? "1" : "0";
        }
        else
        {
            this.Raw[Name] = "" + Value + "";
        }
    }
    
    ParamGetS(Name, X)
    {
        if (this.Raw[this.Case(Name)])
        {
            return this.Raw[this.Case(Name)];
        }
        else
        {
            return X;
        }
    }

    ParamGetI(Name, X)
    {
        if (this.Raw[this.Case(Name)])
        {
            return parseInt(this.Raw[this.Case(Name)]);
        }
        else
        {
            return X;
        }
    }

    ParamGetB(Name, X)
    {
        if (this.Raw[this.Case(Name)])
        {
            let V = this.Raw[this.Case(Name)].toUpperCase();
            if ((V == "1") || (V == "TRUE") || (V == "YES") || (V == "T") || (V == "Y"))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return X;
        }
    }
}


function KeyValue_StringEncode0(Data)
{
    const codeUnits = new Uint16Array(Data.length);
    for (let i = 0; i < codeUnits.length; i++)
    {
        codeUnits[i] = Data.charCodeAt(i);
    }
    return btoa(String.fromCharCode(...new Uint8Array(codeUnits.buffer)));
}

function KeyValue_StringDecode0(Data)
{
    const binary = atob(Data);
    const bytes = new Uint8Array(binary.length);
    for (let i = 0; i < bytes.length; i++)
    {
        bytes[i] = binary.charCodeAt(i);
    }
    return String.fromCharCode(...new Uint16Array(bytes.buffer));
}

function KeyValue_StringEncode(Data)
{
    return btoa(Data);
}

function KeyValue_StringDecode(Data)
{
    return atob(Data);
}

