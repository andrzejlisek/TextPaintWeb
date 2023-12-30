function IndexMeasureSpaceLS()
{
    FileFreeSpaceLS = 0;
    if (LS())
    {
        let DummySize = 1;
        let TestName = "TextPaintTest";
        let MeasuredSize;
        let DummyData;

        while (true)
        {
            DummySize = DummySize * 10;
            DummyData = "".padEnd(DummySize, 'X');
            try
            {
                localStorage.setItem(TestName, DummyData);
                localStorage.removeItem(TestName);
            }
            catch (E)
            {
                DummySize = DummySize / 10;
                break;
            }
        }
        DataDelete(TestName);
        MeasuredSize = DummySize;
        while (DummySize >= 1)
        {
            while (true)
            {
                MeasuredSize = MeasuredSize + DummySize;
                DummyData = "".padEnd(MeasuredSize, 'X');
                try
                {
                    localStorage.setItem(TestName, DummyData);
                    localStorage.removeItem(TestName);
                }
                catch (E)
                {
                    localStorage.removeItem(TestName);
                    MeasuredSize = MeasuredSize - DummySize;
                    DummySize = DummySize / 10;
                    break;
                }
            }
        }
        FileFreeSpaceLS = MeasuredSize + TestName.length;
    }
}


function IndexImportLS()
{
    if (DataExists(FileIndexName))
    {
        FileIndexLS = IndexImport(IndexRaw0, 0);
        FileIndexLSAttr = IndexImport(IndexRaw0, 1);
    }
    IndexMeasureSpaceLS();
    FileLsReady = true;
    ProgInit();
}

