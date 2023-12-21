class ScreenFont
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

    constructor(ImgUrl)
    {
        this.Ready = false;
        this.Idx = [];
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
        }
    }
    
    ImgLoaded()
    {
        this.ImgW = this.Img.width;
        this.ImgH = this.Img.height;
        this.Cnv = document.getElementById("testbmp1");
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
                ConfigFileSet("_WinBitmapFontPage" + IdxI + "", Val);
                this.Idx[IdxI] = Val;
                IdxI++;
                Val0 = Val;
            }
        }
        
        this.CellW = Math.round((this.ImgW - 16) / 256);
        this.CellH = Math.round((this.ImgH) / IdxI);
        this.Blank = this.GetGlyph(32);
        this.Ready = true;
    }
}

