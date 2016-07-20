var __interpretation_started_timestamp__;
var pi = 3.1415926535897931;

var main = function()
{
	var mes = 4;
	var x = 0;
	while (x != 4){
	if (gamepad.isPadPressed(1)) {
				x = [gamepad.padX(0), gamepad.padY(0)][0];
			}
			script.sendMessage(mes);
			script.wait(10);
		}
	script.wait(5000);
	mes = 7;
	while (x != 7){
	if (gamepad.isPadPressed(1)) {
				x = [gamepad.padX(0), gamepad.padY(0)][0];
			}
			script.sendMessage(mes);
			script.wait(10);
		}
	return;
}
