var __interpretation_started_timestamp__;
var pi = 3.1415926535897931;

var x;
var y;

var main = function()
{
	__interpretation_started_timestamp__ = Date.now();
	x = 0;
	y = 0;
	while (!gamepad.isConnected()) {
		script.wait(10);
	}
	while (true) {
		if (gamepad.isPadPressed(1)) {
			x = [gamepad.padX(1), gamepad.padY(1)][0];
			y = [gamepad.padX(1), gamepad.padY(1)][1];
			var x1 = [gamepad.padX(0), gamepad.padY(0)][0];
			var y1 = [gamepad.padX(0), gamepad.padY(0)][1];
			brick.display().addLabel(x,10,25);
			brick.display().addLabel(y,10,40);
			brick.display().addLabel(x1,10,55);
			brick.display().addLabel(y1,10,70);
			brick.display().redraw();
		} 
		script.wait(100);
	}
}
