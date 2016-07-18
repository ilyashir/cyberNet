var __interpretation_started_timestamp__;
var pi = 3.1415926535897931;
function degToRad (deg) { return deg / 180 * Math.PI; }
function radToDeg (rad) { return rad / Math.PI * 180; }
function toSm (dist){return dist*60/1024;}
var Gast,Ga,alpha, last;
var setAng =function(a)
{
	var da=Ga-Gast;
	var step=da/10;
	var t=da/2;
	if (a==1)
	{
		if(da<=5){brick.motor(S5).setPower(Ga); brick.motor(S3).setPower(Ga);}
		else{
			for(var i =1;i<11;i++)
			{
				brick.motor(S5).setPower(Gast+step*i);
				brick.motor(S3).setPower(Gast+step*i);
				script.wait(1);
			} 
		}
	}
	else
	{
		if(da<=5){brick.motor(S1).setPower(Ga); brick.motor(S2).setPower(Ga);}
		else{
			for(var i =1;i<11;i++)
			{
				brick.motor(S1).setPower(Gast+step*i);
				brick.motor(S2).setPower(Gast+step*i);
				script.wait(1);
			} 
		}
	}
}

var main = function()
{
	__interpretation_started_timestamp__ = Date.now();
	Gast=0;
	alpha=0;
	last=0;
	Ga=0;
	var time0=0, dt=0;
	Ga=30;
	setAng(0);
	while(true)
	{
		var Left=brick.sensor(A5).readRawData();
		var Right=brick.sensor(A6).readRawData();
		Left=1023-Left;
		Right=1023-Right;
		var Lsm=toSm(Left);
		var Rsm=toSm(Right);
		if(Math.abs(Lsm-Rsm)>10)
		{
			if(Lsm<Rsm)
			{
				if(Ga<35) Ga+=5;
			}
			else
			{
				if(Ga>-35) Ga-=5;
			}
			time0=-1;
		}
		else
		{
			if(time0==-1)
			{
				time0=script.time();
			}
			else
			{
				dt=script.time()-time0;
			}
			if(dt>3000) Ga=0;
		}
		setAng(1);
		Gast=Ga;
		brick.display().addLabel(Lsm,10,10);
		brick.display().addLabel(Rsm,10,25);
		brick.display().addLabel(dt,10,50);
		brick.display().redraw();
		script.wait(10);
	
	}
	return;
}