var __interpretation_started_timestamp__;
var pi = 3.1415926535897931;
var u1, u2, v = 90, k1 = 20, k2 = 20;

var P_reg = function(){
	var a=brick.sensor("A4").readRawData();
	var b=brick.sensor("A5").readRawData();
	u1=(Math.min(a,b)-600)/k1;
	u2=(-a+b)/k2;
	brick.motor("M1").setPower(-v+u1+u2);
	brick.motor("M2").setPower(-v+u1-u2);
	brick.motor("M3").setPower(v+u1+u2);
	brick.motor("M4").setPower(v+u1-u2);
}

var main = function()
{
	while (true){
		P_reg();
	}
	return;
}