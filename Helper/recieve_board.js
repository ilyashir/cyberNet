var __interpretation_started_timestamp__;
var pi = 3.1415926535897931;

var main = function()
{
	//while (!gamepad.isConnected()) {
	//	script.wait(10);
	//}
	//brick.playSound("media/beep.wav");
	var a = 0;
	brick.motor("S2").setPower(100);
	brick.motor("S1").setPower(-100);
	script.wait(1000);
	while(true)
	{
		if (gamepad.isPadPressed(1)) {
			var x0 = [gamepad.padX(0), gamepad.padY(0)][0]*1;
			var y0 = [gamepad.padX(0), gamepad.padY(0)][1]*1;
			var x1 = [gamepad.padX(1), gamepad.padY(1)][0]*1;
			var y1 = [gamepad.padX(1), gamepad.padY(1)][1]*1;
			print(x0);
			a++;
			brick.display().addLabel(x0,10,25);
			brick.display().addLabel(y0,10,40);
			brick.display().addLabel(x1,10,55);
			brick.display().addLabel(y1,10,70);
			brick.display().redraw();
			if(x0>180)
				x0 = x0 - 360;
		}
		if(y0<5)
			x0=y0=0;
		var power = 1 * x0;
		var base = 10 * y0;
		if(Math.abs(x0)>15)
			base = 0;
		brick.motor("S2").setPower( -base + power+10);
		brick.motor("S1").setPower( base +power+10);
	}
	return;
}
