var x;
var y;
var main = function()
{
	//script.system("mjpg_streamer -i \"input_uvc.so -d /dev/video2 -r 432x240 -f 30\" -o \"output_http.so -w /usr/share/mjpg-streamer/www/\"");
	brick.motor("S3").setPower(-30);
		while (!gamepad.isConnected())
			script.wait(10);
		while (!brick.keys().wasPressed(KeysEnum.Esc)) {
			var p = gamepad.isPadPressed(0);
			if (p) {
				x = gamepad.padX(0);
				gamepad.reset();
				//y = gamepad.padX(0);
				brick.display().addLabel("x=" + x + " p=" + p, 0, 0);
				//brick.display().addLabel(y, 0, 15);
				brick.display().redraw();
				if(x!=0){
					x = 0;
					brick.motor("S3").setPower(-90);
					brick.display().addLabel("поднял", 0 , 30);
					brick.display().redraw();
					script.wait(8000);
					brick.motor("S3").setPower(-30);
					brick.display().addLabel("опустил", 0 , 30);
					brick.display().redraw();
					script.wait(4000);
				}
				
			}
			script.wait(200);
		}
}