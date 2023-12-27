let _ProgInit;
let _ProgEventKey;
let _ProgEventTick;
let ProgEventOther;
let ProgEventOtherFile;

function ProgStart()
{
    ProgStarted = true;
    
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

function _ProgCallback(D)
{
    let I = 0;
    let Proc = D[0];
    {
        while (Proc != 0)
        {
            switch (Proc)
            {
                case 98:
                    _ProgInit("2");
                    I += 1;
                    break;
                case 99:
                    ScreenInit1();
                    I += 1;
                    break;
                case 100:
                    ProgStart();
                    I += 1;
                    break;
                case 101:
                    ScreenChar(D[I+1],D[I+2],D[I+3],D[I+4],D[I+5],D[I+6],D[I+7],D[I+8]);
                    I += 9;
                    break;
                case 102:
                    ScreenClear(D[I+1],D[I+2]);
                    I += 3;
                    break;
                case 103:
                    ScreenResize(D[I+1],D[I+2]);
                    I += 3;
                    break;
                case 104:
                    ScreenCursorMove(D[I+1],D[I+2]);
                    I += 3;
                    break;
                case 105:
                    ScreenTextMove(D[I+1],D[I+2],D[I+3],D[I+4],D[I+5],D[I+6]);
                    I += 7;
                    break;
                case 106:
                    ScreenLineOffset(D[I+1],D[I+2],D[I+3],D[I+4],D[I+5],D[I+6]);
                    I += 7;
                    break;
                case 111:
                    FileImport(D[I+1],D[I+2],StringBufDecode(D[I+3]),D[I+4]);
                    I += 5;
                    break;
                case 112:
                    FileExport(D[I+1],D[I+2],StringBufDecode(D[I+3]),D[I+4],D[I+5]);
                    I += 6;
                    break;
                case 113:
                    ConfigFileGet(D[I+1],D[I+2]);
                    if (ProgStarted)
                    {
                        ScreenSetDisplayConfig(true);
                    }
                    I += 3;
                    break;
                case 200:
                    VTTEST_.VTData(D[I+1]);
                    I += 2;
                    break;
                case 201:
                    VTTEST_.VTStart();
                    I += 2;
                    break;
                case 202:
                    VTTEST_.VTStop();
                    I += 2;
                    break;
            }
            Proc = D[I];
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
          console.log("Status: " + text);
          
          if (text == "")
          {
            ProgLoaded = true;
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


ProgInit();
FileDbOpen();

