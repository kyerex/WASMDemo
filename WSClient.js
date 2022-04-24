
new WSClient(""); // define prototypes

WSClient.prototype.receiver=null;
WSClient.prototype.send=send;
WSClient.prototype.close=close;


function WSClient(url)
{
    if (url == "") return;
    this.sock = new WebSocket(url);
    this.sock.binaryType = "arraybuffer";
	this.sock.onclose = function(e) {
		console.log("Web Socket closed");
	};

	this.sock.onopen = function (e){
        console.log("web Socket open");
    };
    
    this.sock.onmessage=function (e){
		ws.receiver (e.data);
        ws.sock.send("got it\0");  // ack
    }
}
function send(request,receiver)
{
    this.receiver=receiver;
    this.sock.send(request);
}

function close()
{
    this.sock.close();
}