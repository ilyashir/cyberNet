var pi = 3.1415926535897931;
var length = 0;
var out = 0;
var x=0;
var y=-1;
var que = true;
var main = function()
{
	Struc(0, 700, 200, 1);
	return;
}
var request = function(){
	script.sendMessage(13);
	while (!gamepad.isConnected())
		script.wait(10);
	while (true) {
		if (gamepad.isPadPressed(0)) {
			x = gamepad.padX(0);
			y = gamepad.padX(0);
			brick.display().addLabel(x, 0, 0);
			brick.display().addLabel(y, 0, 15);
			brick.display().redraw();
			break;
		}
		script.wait(100);
	}
	switch(x){
		case 1: stay1(); break;
		case 2: stay2(); break;
		case 3: stay3(); break;
		case 4: stay4(); break;
		case 5: stay5(); break;
		case 6: stay6(); break;
	}
}
var end = function(){
	brick.motor('S1').setPower(0);
	brick.motor('S2').setPower(0);
	script.wait(1000);
}
var fullSetPower = function(z){
	brick.motor('M1').setPower(z);
	brick.motor('M2').setPower(z);
	brick.motor('M3').setPower(z);
	brick.motor('M4').setPower(z);
}
var EnReset = function(){
	brick.encoder("E1").reset();
	brick.encoder("E2").reset();
	brick.encoder("E3").reset();
	brick.encoder("E4").reset();
}
var Struc = function(angle, ms, len, y){
	brick.motor('S1').setPower(angle);
	brick.motor('S2').setPower(angle);
	script.wait(ms);
	length = len;
	y==1?go():back();
}
var go = function()
{
	out = 0;
	EnReset();
	out = 360*length/80*2.5;
	for (i=0; i <= out; i=i+1-1)
	{
		i = (Math.abs(brick.encoder("E1").read() + brick.encoder("E2").read()))/2;
		if(brick.sensor("A1").read()<10){
			fullSetPower(0);
			if(que=true){
				que=false;
				request();
				break;
			}
		}
		else
			fullSetPower(100);
	}
	script.wait (100);
	fullSetPower(0);
}
var back = function()
{
	out = 0;
	EnReset();
	out = 360*length/80*2.5;
	for (i=0; i <= out; i=i+1-1)
	{
		i = (Math.abs(brick.encoder("E1").read() + brick.encoder("E2").read()))/2;
		fullSetPower(-100);
	}
	script.wait (100);
	fullSetPower(0);
}
var stay6 = function()
{
	Struc(-9, 600, 69, 1);
	Struc(60, 600, 60, 1);
	Struc(0, 600, 1, 1);
	end();
}
var stay5 = function()
{
	Struc(0, 500, 75, 1);
	Struc(-60, 700, 48, 1);
	Struc(0, 500, 29, 1);
	end();
}
var stay4 = function()
{
	Struc(0, 500, 49, 1);
	Struc(-60, 700, 42, 1);
	Struc(3, 700, 73, 0);
	end();
}
var stay3 = function()
{
	Struc(0, 500, 45, 1);
	Struc(-60, 700, 50, 1);
	Struc(0, 500, 25, 1);
	end();
}
var stay2 = function()
{
	Struc(0, 500, 70, 1);
	Struc(60, 700, 48, 0);
	Struc(0, 500, 17, 0);
	end();
}
var stay1 = function()
{
	Struc(0, 500, 5, 1);
	Struc(-60, 700, 50, 1);
	Struc(0, 500, 25, 1);
	end();
}
main();
