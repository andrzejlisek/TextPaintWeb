let FileDb;
let FileDbName = "Files";

function FileDbOpen()
{
    const Req = window.indexedDB.open('TextPaint', 1);

    Req.addEventListener('error', () => console.error('FileDb: Database failed to open'));

    Req.addEventListener('success', () => {
        FileDb = Req.result;
        FileDbOpenComplete(true);
    });

    Req.addEventListener('upgradeneeded', e => {
        FileDb = e.target.result;

        const ObjStore = FileDb.createObjectStore(FileDbName, { keyPath: 'id' });

        FileDbOpenComplete(false);
    });
}


function FileDbOpenComplete(GetIndex)
{
    if (GetIndex)
    {
        FileDbGet(0);
    }
}

function FileDbGet(ItemId, EvtId, Kind, Name, Attrib)
{
    const ObjStore = FileDb.transaction(FileDbName).objectStore(FileDbName);
    let NoDataRead = true;
    ObjStore.openCursor(IDBKeyRange.bound(ItemId, ItemId)).addEventListener('success', e => {


        const cursor = e.target.result;
        if(cursor)
        {
            NoDataRead = false;
            if (ItemId == 0)
            {
                FileIndexDB = IndexImport(cursor.value.data, 0);
                FileIndexDBAttr = IndexImport(cursor.value.data, 1);
                FileDbReady = true;
                ProgInit();
            }
            else
            {
                let Data = cursor.value.data;
                FileImportFinish(EvtId, Kind, Name, Attrib, Data, 0);
            }

            cursor.continue();
        }
        else
        {
            if (NoDataRead && (ItemId == 0))
            {
                if (ItemId == 0)
                {
                    FileDbReady = true;
                    ProgInit();
                }
                else
                {
                    FileImportFinish(EvtId, Kind, Name, Attrib, "", 0);
                }
            }
        }
    });
}



function FileDbSet(ItemId, EvtId, Kind, Name, Attrib, Data)
{
    let Preview = "";
    if (ItemId != 0)
    {
        Preview = FileCreatePreview(Data);
    }

    const Item = { id: ItemId, name: Name, data: Data, attrib: Attrib, preview: Preview };
    const DbTrans = FileDb.transaction([FileDbName], 'readwrite');
    const ObjStore = DbTrans.objectStore(FileDbName);
    const Req = ObjStore.put(Item);

    Req.addEventListener('success', () => {
    });

    DbTrans.addEventListener('complete', () => {
        if (ItemId != 0)
        {
            FileExportFinish(EvtId, Kind, Name, Attrib, Data, 0);
        }
    });

    DbTrans.addEventListener('error', () => {
        if (ItemId != 0)
        {
            FileExportFinish(EvtId, Kind, Name, Attrib, Data, 1);
        }
    });
}


function FileDbDelete(ItemId, EvtId, Kind, Name, Attrib)
{
    const DbTrans = FileDb.transaction([FileDbName], 'readwrite');
    const ObjStore = DbTrans.objectStore(FileDbName);
    const Req = ObjStore.delete(IDBKeyRange.bound(ItemId, ItemId));

    DbTrans.addEventListener('complete', () => {
        if (EvtId > 0)
        {
            FileExportFinish(EvtId, Kind, Name, Attrib, "", 0);
        }
    });
}

