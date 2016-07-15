var __interpretation_started_timestamp__;
var pi = 3.1415926535897931;

var main = function()
{
	//while (!gamepad.isConnected()) {
	//	script.wait(10);
	//}
	//brick.playSound("media/beep.wav");
	while(true)
	{
		if (gamepad.isPadPressed(1)) {
			var x0 = [gamepad.padX(0), gamepad.padY(0)][0]*1;
			var y0 = [gamepad.padX(0), gamepad.padY(0)][1]*1;
			var x1 = [gamepad.padX(1), gamepad.padY(1)][0]*1;
			var y1 = [gamepad.padX(1), gamepad.padY(1)][1]*1;
			print(x0);
			brick.display().addLabel(x0,10,25);
			brick.display().addLabel(y0,10,40);
			brick.display().addLabel(x1,10,55);
			brick.display().addLabel(y1,10,70);
			brick.display().redraw();
			if(x0>180)
				x0 = x0 - 360;
		}
	}
	return;
}
