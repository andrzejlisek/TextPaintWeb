let _ProgInit;
let _ProgEventKey;
let _ProgEventTick;
let ProgEventOther;
let ProgEventOtherFile;

function ProgStart()
{
    ScreenTimerCallback = ScreenTimerPeriod;
    ProgStarted = true;
    ProgInitScreen();
    ScreenTimerStart();
    
    KeybInit();
}

function ProgEventTick()
{
    if (ProgStarted)
    {
        _ProgEventTick();
    }
}

function VTTestData(X)
{
    ProgEventOther("Received", X, 0, 0, 0, 0);
}

function ProgEventKey(KeyName, KeyChr, KeyS, KeyC, KeyA)
{
    if (ProgStarted)
    {
        _ProgEventKey(KeyName, KeyChr, KeyS, KeyC, KeyA);
    }
}

let CallbackQueue = [];
let CallbackQueueI = 0;

function _ProgCallback(D)
{
    if (D.length > 1)
    {
        CallbackQueue.push(D);
    }
    const TimeLimit = performance.now() + ScreenTimerCallback;
    let AtLeastOneAction = true;
    while ((CallbackQueue.length > 0) && ((TimeLimit > performance.now()) || AtLeastOneAction))
    {
        AtLeastOneAction = false;
        const D = CallbackQueue[0];
        const Proc = D[CallbackQueueI];
        switch (Proc)
        {
            case 0:
                CallbackQueue.shift();
                CallbackQueueI = 0;
                break;
            case 98:
                _ProgInit("2");
                CallbackQueueI += 1;
                break;
            case 99:
                ProgInitialized = true;
                ProgInitScreen();
                ScreenInit1();
                CallbackQueueI += 1;
                break;
            case 100:
                ProgStart();
                CallbackQueueI += 1;
                break;
            case 101:
                ScreenChar(D[CallbackQueueI+1],D[CallbackQueueI+2],D[CallbackQueueI+3],D[CallbackQueueI+4],D[CallbackQueueI+5],D[CallbackQueueI+6],D[CallbackQueueI+7],D[CallbackQueueI+8]);
                CallbackQueueI += 9;
                break;
            case 102:
                ScreenClear(D[CallbackQueueI+1],D[CallbackQueueI+2]);
                CallbackQueueI += 3;
                break;
            case 103:
                ScreenResize(D[CallbackQueueI+1],D[CallbackQueueI+2]);
                CallbackQueueI += 3;
                break;
            case 104:
                ScreenCursorMove(D[CallbackQueueI+1],D[CallbackQueueI+2]);
                CallbackQueueI += 3;
                break;
            case 105:
                ScreenTextMove(D[CallbackQueueI+1],D[CallbackQueueI+2],D[CallbackQueueI+3],D[CallbackQueueI+4],D[CallbackQueueI+5],D[CallbackQueueI+6]);
                CallbackQueueI += 7;
                break;
            case 106:
                ScreenLineOffset(D[CallbackQueueI+1],D[CallbackQueueI+2],D[CallbackQueueI+3],D[CallbackQueueI+4],D[CallbackQueueI+5],D[CallbackQueueI+6]);
                CallbackQueueI += 7;
                break;
            case 111:
                FileImport(D[CallbackQueueI+1],D[CallbackQueueI+2],StringBufDecode(D[CallbackQueueI+3]),D[CallbackQueueI+4]);
                CallbackQueueI += 5;
                break;
            case 112:
                FileExport(D[CallbackQueueI+1],D[CallbackQueueI+2],StringBufDecode(D[CallbackQueueI+3]),D[CallbackQueueI+4],D[CallbackQueueI+5]);
                CallbackQueueI += 6;
                break;
            case 113:
                ConfigFileGet(StringBufDecode(D[CallbackQueueI+1]),StringBufDecode(D[CallbackQueueI+2]));
                if (ProgStarted)
                {
                    ScreenSetDisplayConfig(true);
                }
                CallbackQueueI += 3;
                break;
            case 200:
                VTTEST_.VTData(D[CallbackQueueI+1]);
                CallbackQueueI += 2;
                break;
            case 201:
                VTTEST_.VTStart();
                CallbackQueueI += 2;
                break;
            case 202:
                VTTEST_.VTStop();
                CallbackQueueI += 2;
                break;
        }
    }
}


      var Module = {
        print: (function() {
          return function(text) {
            if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
            // These replacements are necessary if you render to raw HTML
            //text = text.replace(/&/g, "&amp;");
            //text = text.replace(/</g, "&lt;");
            //text = text.replace(/>/g, "&gt;");
            //text = text.replace('\n', '<br>', 'g');
            console.log(text);
          };
        })(),
        canvas: (() => {
          var canvas = document.getElementById('canvas');

          // As a default initial behavior, pop up an alert when webgl context is lost. To make your
          // application robust, you may want to override this behavior before shipping!
          // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
          //canvas.addEventListener("webglcontextlost", (e) => { alert('WebGL context lost. You will need to reload the page.'); e.preventDefault(); }, false);

          return canvas;
        })(),
        setStatus: (text) => {
          if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: '' };
          if (text === Module.setStatus.last.text) return;
          var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
          var now = Date.now();
          if (m && now - Module.setStatus.last.time < 30) return; // if this is a progress update, skip it if too soon
          Module.setStatus.last.time = now;
          Module.setStatus.last.text = text;
          if (m) {
            text = m[1];
            //progressElement.value = parseInt(m[2])*100;
            //progressElement.max = parseInt(m[4])*100;
            //progressElement.hidden = false;
            //spinnerElement.hidden = false;
          } else {
            //progressElement.value = null;
            //progressElement.max = null;
            //progressElement.hidden = true;
            //if (!text) spinnerElement.style.display = 'none';
          }
          //console.log("Status: " + text);
          
          if (text == "")
          {
            ProgLoaded = true;
            ProgInit();
          }
        },
        totalDependencies: 0,
        monitorRunDependencies: (left) => {
          this.totalDependencies = Math.max(this.totalDependencies, left);
          Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies-left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
        }
      };
      Module.setStatus('Downloading...');
      window.onerror = (event) => {
        // TODO: do not warn on ok events like simulating an infinite loop or exitStatus
        Module.setStatus('Exception thrown, see JavaScript console');
        //spinnerElement.style.display = 'none';
        Module.setStatus = (text) => {
          if (text) console.error('[post-exception status] ' + text);
        };
      };


ProgInitScreen();
FileDbOpen();
IndexImportLS();

