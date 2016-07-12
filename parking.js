var pi = 3.1415926535897931;
var length = 0;
var out = 0;

var main = function()
{
	stay3();
	return;
}
var end = function(){
	brick.motor('S1').setPower(0);
	brick.motor('S2').setPower(0);
	script.wait(1000);
}
var fullSetPower = function(x){
	brick.motor('M1').setPower(x);
	brick.motor('M2').setPower(x);
	brick.motor('M3').setPower(x);
	brick.motor('M4').setPower(x);
}
var EnReset = function(){
	brick.encoder("E1").reset();
	brick.encoder("E2").reset();
	brick.encoder("E3").reset();
	brick.encoder("E4").reset();
}
var Struc = function(power, ms, len, y){
	brick.motor('S1').setPower(power);
	brick.motor('S2').setPower(power);
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
