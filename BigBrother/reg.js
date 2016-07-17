var __interpretation_started_timestamp__;
var pi = 3.1415926535897931;
var u1, u2, v = -90, k1 = 20, k2 = 20, r=4, rr;

var p_reg = function(){
	var a=brick.sensor("A4").readRawData();
	var b=brick.sensor("A5").readRawData();
	u1=(Math.min(a,b)-600)/k1;
	u2=(-a+b)/k2;
	brick.motor("M1").setPower(-v+u1+u2);
	brick.motor("M2").setPower(-v+u1-u2);
	brick.motor("M3").setPower(v+u1+u2);
	brick.motor("M4").setPower(v+u1-u2);
	script.wait(50);
}

var go_on_edge = function(dist)
{
	brick.encoder("E1").reset();
	brick.encoder("E2").reset();
	brick.encoder("E3").reset();
	brick.encoder("E4").reset();
	var a=0,b=0,c=0,d=0;
	if(v>0)
		light = brick.sensor("A1").readRawData();
	else
		light = brick.sensor("A2").readRawData();
	while (Math.min(Math.min(a,b),Math.min(c,d))/180.0*pi*4/Math.sqrt(2.0)<dist && light < 600){
		if(v>0)
			light = brick.sensor("A1").readRawData();
		else
			light = brick.sensor("A2").readRawData();
		p_reg();
		a=Math.abs(brick.encoder("E1").read());
		b=Math.abs(brick.encoder("E2").read());
		c=Math.abs(brick.encoder("E3").read());
		d=Math.abs(brick.encoder("E4").read());
	}
	brick.motor("M1").powerOff();
	brick.motor("M2").powerOff();
	brick.motor("M3").powerOff();
	brick.motor("M4").powerOff();
}

var rotate = function(ang)
{
	brick.encoder("E1").reset();
	brick.encoder("E2").reset();
	brick.encoder("E3").reset();
	brick.encoder("E4").reset();
	var a=0,b=0,c=0,d=0;
	while (Math.min(Math.min(a,b),Math.min(c,d))/6.9<ang){
		brick.motor("M1").setPower(v);
		brick.motor("M2").setPower(-v);
		brick.motor("M3").setPower(v);
		brick.motor("M4").setPower(-v);
		a=Math.abs(brick.encoder("E1").read());
		b=Math.abs(brick.encoder("E2").read());
		c=Math.abs(brick.encoder("E3").read());
		d=Math.abs(brick.encoder("E4").read());
	}
	brick.motor("M1").powerOff();
	brick.motor("M2").powerOff();
	brick.motor("M3").powerOff();
	brick.motor("M4").powerOff();
}

var go = function(dist)
{
	brick.encoder("E1").reset();
	brick.encoder("E2").reset();
	brick.encoder("E3").reset();
	brick.encoder("E4").reset();
	var a=0,b=0,c=0,d=0;
	while (Math.min(Math.min(a,b),Math.min(c,d))/180.0*pi*4/Math.sqrt(2.0)<dist && light < 600){
		brick.motor("M1").setPower(v);
		brick.motor("M2").setPower(v);
		brick.motor("M3").setPower(v);
		brick.motor("M4").setPower(v);
		a=Math.abs(brick.encoder("E1").read());
		b=Math.abs(brick.encoder("E2").read());
		c=Math.abs(brick.encoder("E3").read());
		d=Math.abs(brick.encoder("E4").read());
	}
	brick.motor("M1").powerOff();
	brick.motor("M2").powerOff();
	brick.motor("M3").powerOff();
	brick.motor("M4").powerOff();
}

var main = function()
{
	rotate(360);
	script.wait(100);
	return;
}