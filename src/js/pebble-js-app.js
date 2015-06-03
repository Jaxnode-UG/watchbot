// Function to send a message to the Pebble using AppMessage API
function sendMessage() {
	Pebble.sendAppMessage({"status": 0});
	
	// PRO TIP: If you are sending more than one message, or a complex set of messages, 
	// it is important that you setup an ackHandler and a nackHandler and call 
	// Pebble.sendAppMessage({ /* Message here */ }, ackHandler, nackHandler), which 
	// will designate the ackHandler and nackHandler that will be called upon the Pebble 
	// ack-ing or nack-ing the message you just sent. The specified nackHandler will 
	// also be called if your message send attempt times out.
}


// Called when JS is ready
Pebble.addEventListener("ready",
							function(e) {
							});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage", function(e) {
	//{"status":1,"y-axis":-512,"x-axis":-88,"z-axis":-736}
	var req = new XMLHttpRequest();
	req.open('GET', 'http://pebbleaccel.azurewebsites.net/io/accel/' + e.payload['x-axis'] + '/' + e.payload['y-axis'] + '/' + e.payload['z-axis'], true);
	req.onload = function(e) {
  if (req.readyState == 4 && req.status == 200) {
    if(req.status !== 200) {
				console.log('Error'); 
			}
  	}
	}
	req.send(null);
});
