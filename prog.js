let ProgLoaded = false;
let ProgStarted = false;

let Stopwatch = 0;

let VTTEST_ = document.getElementById("vttest").contentWindow;

function StopwatchReset()
{
    console.log("<<<");
    Stopwatch = performance.now();
}

function StopwatchTime()
{
    let StopwatchTime = performance.now() - Stopwatch;
    console.log(">>> " + StopwatchTime);
}

function ProgInit()
{
    if (ProgLoaded)
    {
        Module.ccall("Init", null, null, null);
    }
    else
    {
        setTimeout(ProgInit, 20);
    }
}

function ProgStart()
{
    ProgStarted = true;
}

function ProgEventTick()
{
    if (ProgStarted)
    {
        Module.ccall("EventTick", null, null, null);
    }
}

function VTTestData(X)
{
    Module.ccall("EventOther", null, ["string", "string", "number", "number", "number", "number"], ["Received", X, 0, 0, 0, 0]);
}

function ProgEventKey(KeyName, KeyChr, KeyS, KeyC, KeyA)
{
    if (ProgStarted)
    {
        Module.ccall("EventKey", null, ["string", "number", "number", "number", "number"], [KeyName, KeyChr, KeyS, KeyC, KeyA]);
    }
}

function ProgTestProc()
{
    let Napis = "1ą🬔ę2";
    //Napis = "qwerty";
    let Napis2 = Module.ccall("EventOther", null, ["string", "string", "number", "number", "number", "number"], ["TextPaste", Napis, Napis.length > 0 ? 1 : 0, 0, 0, 0]);
    //alert(Napis2);
}

function ProgTestVT()
{
    
    //document.getElementById('targetFrame').contentWindow.targetFunction();

    //Module.ccall("EventVT0", null, ["string"], ["TextPaste"]);
}

function TestOK()
{
    if (ProgStarted)
    {
    }
}

function _TestOK2(D)
{
    let I = 0;
    let Proc = D[0];
    {
        while (Proc != 0)
        {
            switch (Proc)
            {
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

function _TestOK3(D)
{
    console.log(D);
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


