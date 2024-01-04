class ScreenFontBmp
{
    GetPixelBin(X, Y)
    {
        let Ptr = (this.ImgW * Y + X) << 2;
        if ((this.Data.data[Ptr + 0] + this.Data.data[Ptr + 1] + this.Data.data[Ptr + 2]) >= 384)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    GetGlyph(Chr)
    {
        let ChrP = Chr >> 8;
        let ChrN = Chr % 256;
        let Idx0 = this.Idx.indexOf(ChrP);
        if (Idx0 >= 0)
        {
            let Vals = [];
            let Ptr = 0;
            for (let Y = 0; Y < this.CellH; Y++)
            {
                for (let X = 0; X < this.CellW; X++)
                {
                    Vals[Ptr] = this.GetPixelBin(16 + (ChrN * this.CellW) + X, (Idx0 * this.CellH) + Y);
                    Ptr++;
                }
            }
            return Vals;
        }
        else
        {
            return this.Blank;
        }
    }

    GetGlyphDbl(Chr)
    {
        let ChrP = Chr >> 8;
        let ChrN = Chr % 256;
        let Idx0 = this.Idx.indexOf(ChrP);
        if (Idx0 >= 0)
        {
            let Vals = [];
            let Ptr = 0;
            for (let Y = 0; Y < this.CellH; Y++)
            {
                for (let X = 0; X < this.CellW; X++)
                {
                    Vals[(Ptr << 1) + 0] = this.GetPixelBin(16 + (ChrN * this.CellW) + X, (Idx0 * this.CellH) + Y);
                    Vals[(Ptr << 1) + 1] = Vals[(Ptr << 1) + 0];
                    Ptr++;
                }
            }
            return Vals;
        }
        else
        {
            return this.Blank;
        }
    }

    GetGlyphLeft(Chr)
    {
        let ChrP = Chr >> 8;
        let ChrN = Chr % 256;
        let Idx0 = this.Idx.indexOf(ChrP);
        if (Idx0 >= 0)
        {
            let Vals = [];
            let Ptr = 0;
            for (let Y = 0; Y < this.CellH; Y++)
            {
                for (let X = 0; X < this.CellW2; X++)
                {
                    Vals[Ptr] = this.GetPixelBin(16 + (ChrN * this.CellW) + X, (Idx0 * this.CellH) + Y);
                    Ptr++;
                }
            }
            return Vals;
        }
        else
        {
            return this.Blank;
        }
    }

    GetGlyphRight(Chr)
    {
        let ChrP = Chr >> 8;
        let ChrN = Chr % 256;
        let Idx0 = this.Idx.indexOf(ChrP);
        if (Idx0 >= 0)
        {
            let Vals = [];
            let Ptr = 0;
            for (let Y = 0; Y < this.CellH; Y++)
            {
                for (let X = 0; X < this.CellW2; X++)
                {
                    Vals[Ptr] = this.GetPixelBin(16 + (ChrN * this.CellW) + X + this.CellW2, (Idx0 * this.CellH) + Y);
                    Ptr++;
                }
            }
            return Vals;
        }
        else
        {
            return this.Blank;
        }
    }

    constructor(ImgUrl, CallbackFunction_, CallbackSender_, CallbackParam_)
    {
        this.CallbackSender = CallbackSender_;
        this.CallbackFunction = CallbackFunction_;
        this.CallbackParam = CallbackParam_;
    
        this.Ready = false;
        this.Idx = [];
        this.Pages = [];
        if (ImgUrl)
        {
            this.Img = new Image();
            this.Img.onload = function() { this.parent.ImgLoaded() };
            this.Img.parent = this;
            this.Img.src = ImgUrl;
        }
        else
        {
            this.Img = null;
            this.Ready = true;
            this.CallbackFunction(this.CallbackSender, this.CallbackParam);
        }
    }
    
    CreateBlank(CellW_, CellH_)
    {
        this.Blank = [];
        const WH = CellW_ * CellH_;
        for (let I = 0; I < WH; I++)
        {
            this.Blank[I] = 0;
        }
    }
    
    ImgLoaded()
    {
        this.ImgW = this.Img.width;
        this.ImgH = this.Img.height;
        this.Cnv = document.getElementById("fontbmp");
        this.Cnv.width = this.ImgW;
        this.Cnv.height = this.ImgH;
        this.Ctx = this.Cnv.getContext('2d');
        this.Ctx.drawImage(this.Img, 0, 0);
        this.Data = this.Ctx.getImageData(0, 0, this.ImgW, this.ImgH);
        let IdxI = 0;
        let Val0 = -1;
        for (let I = 0; I < this.Img.height; I++)
        {
            let Val = 0;
            Val += (this.GetPixelBin( 0, I) << 15);
            Val += (this.GetPixelBin( 1, I) << 14);
            Val += (this.GetPixelBin( 2, I) << 13);
            Val += (this.GetPixelBin( 3, I) << 12);
            Val += (this.GetPixelBin( 4, I) << 11);
            Val += (this.GetPixelBin( 5, I) << 10);
            Val += (this.GetPixelBin( 6, I) <<  9);
            Val += (this.GetPixelBin( 7, I) <<  8);
            Val += (this.GetPixelBin( 8, I) <<  7);
            Val += (this.GetPixelBin( 9, I) <<  6);
            Val += (this.GetPixelBin(10, I) <<  5);
            Val += (this.GetPixelBin(11, I) <<  4);
            Val += (this.GetPixelBin(12, I) <<  3);
            Val += (this.GetPixelBin(13, I) <<  2);
            Val += (this.GetPixelBin(14, I) <<  1);
            Val += (this.GetPixelBin(15, I));
            if (Val0 != Val)
            {
                this.Pages.push(Val);
                this.Idx[IdxI] = Val;
                IdxI++;
                Val0 = Val;
            }
        }
        
        this.CellW = Math.round((this.ImgW - 16) / 256);
        this.CellH = Math.round((this.ImgH) / IdxI);
        this.CellW2 = this.CellW / 2;
        this.CreateBlank(this.CellW, this.CellH);
        this.Ready = true;
        this.CallbackFunction(this.CallbackSender, this.CallbackParam);
    }
}


class ScreenFontDbl
{
    constructor(DblUrl, CallbackFunction_, CallbackSender_)
    {
        this.CallbackSender = CallbackSender_;
        this.CallbackFunction = CallbackFunction_;
        if (DblUrl != "")
        {
            fetch(DblUrl)
            .then(_1 => _1.text())
            .then(_2 => this.FileLoaded(_2));
        }
        else
        {
            this.FileLoaded("");
        }
    }


    FileLoaded(Raw)
    {
        this.DblList1 = [];
        this.DblList2 = [];
        this.DblListRaw = Raw.split("\n");

        for (let I = 0; I < this.DblListRaw.length; I++)
        {
            const II = this.DblListRaw[I].indexOf("..");
            if (II > 0)
            {
                this.DblList1.push(HexToNum(this.DblListRaw[I].substr(0, II)));
                this.DblList2.push(HexToNum(this.DblListRaw[I].substr(II + 2)));
            }
            else
            {
                if (this.DblListRaw[I].length >= 2)
                {
                    this.DblList1.push(HexToNum(this.DblListRaw[I]));
                    this.DblList2.push(HexToNum(this.DblListRaw[I]));
                }
            }
        }
        
        this.CallbackFunction(this.CallbackSender, 3);
    }
}


class ScreenFontCplx
{
    constructor(ImgUrl1, ImgUrl2, DblUrl, Mode, CallbackFunction_)
    {
        this.Mode = Mode;
        this.Ready1 = false;
        this.Ready2 = false;
        this.Ready3 = false;
        this.CallbackFunction = CallbackFunction_;
        this.Font1 = new ScreenFontBmp(ImgUrl1, this.constCallback, this, 1);
        this.Font2 = new ScreenFontBmp(this.Mode > 0 ? ImgUrl2 : "", this.constCallback, this, 2);
        this.Dbl = new ScreenFontDbl(DblUrl, this.constCallback, this);
    }
    
    constCallback(Sender, N)
    {
        if (N == 1) { Sender.Ready1 = true; }
        if (N == 2) { Sender.Ready2 = true; }
        if (N == 3) { Sender.Ready3 = true; }
        if (Sender.Ready1 && Sender.Ready2 && Sender.Ready3)
        {
            let Pages = [];
            for (var I = 0; I < Sender.Font1.Pages.length; I++)
            {
                if (Pages.indexOf(Sender.Font1.Pages[I]) < 0)
                {
                    Pages.push(Sender.Font1.Pages[I]);
                }
            }

            for (var I = 0; I < Sender.Font2.Pages.length; I++)
            {
                if (Pages.indexOf(Sender.Font2.Pages[I]) < 0)
                {
                    Pages.push(Sender.Font2.Pages[I]);
                }
            }
            
            Pages.sort(function(a, b){return a - b});

            ConfigFileSet("_WinBitmapFontPageList", Pages.join("|"));

            Sender.DblList1 = Sender.Dbl.DblList1;
            Sender.DblList2 = Sender.Dbl.DblList2;
            Sender.DblListL = Sender.DblList1.length;
            
            if (Sender.DblListL == 0)
            {
                Sender.Mode = 0;
            }

            Sender.CellW = Sender.Font1.CellW;
            Sender.CellH = Sender.Font1.CellH;
            
            Sender.Font1.CreateBlank(Sender.CellW, Sender.CellH);
            Sender.Font2.CreateBlank(Sender.CellW << 1, Sender.CellH);
            
            switch (Sender.Mode)
            {
                case 0:
                    Sender.GetGlyph = Sender.GetGlyph0;
                    ConfigFileSet("_WinBitmapFontDouble1", "");
                    ConfigFileSet("_WinBitmapFontDouble2", "");
                    break;
                case 1:
                    Sender.GetGlyph = Sender.GetGlyph1;
                    ConfigFileSet("_WinBitmapFontDouble1", Sender.DblList1.join("|"));
                    ConfigFileSet("_WinBitmapFontDouble2", Sender.DblList2.join("|"));
                    break;
                case 2:
                    Sender.CellW = Sender.Font1.CellW << 1;
                    Sender.GetGlyph = Sender.GetGlyph2;
                    ConfigFileSet("_WinBitmapFontDouble1", "");
                    ConfigFileSet("_WinBitmapFontDouble2", "");
                    break;
            }
            Sender.CallbackFunction();
        }
    }


    GetGlyph0(Chr)
    {
        return this.Font1.GetGlyph(Chr);
    }

    GetGlyph1(Chr)
    {
        if (Chr >= 0x110000)
        {
            Chr -= 0x110000;
            for (let I = 0; I < this.DblListL; I++)
            {
                if ((Chr >= this.DblList1[I]) && (Chr <= this.DblList2[I]))
                {
                    return this.Font2.GetGlyphRight(Chr);
                }
            }
        }
        else
        {
            for (let I = 0; I < this.DblListL; I++)
            {
                if ((Chr >= this.DblList1[I]) && (Chr <= this.DblList2[I]))
                {
                    return this.Font2.GetGlyphLeft(Chr);
                }
            }
        }
        return this.Font1.GetGlyph(Chr);
    }
    
    GetGlyph2(Chr)
    {
        for (let I = 0; I < this.DblListL; I++)
        {
            if ((Chr >= this.DblList1[I]) && (Chr <= this.DblList2[I]))
            {
                return this.Font2.GetGlyph(Chr);
            }
        }
        return this.Font1.GetGlyphDbl(Chr);
    }
}

