var __interpretation_started_timestamp__;
//Perevod v santimetry
function toSm (dist){return dist*60/1024;}

var camera;//Ugol povorota camery

var manip = function(){
	brick.encoder("E1").reset();
	brick.encoder("E3").reset();
	while(brick.encoder('E3').read()>=-750){
		brick.motor('M3').setPower(-100);
		script.wait(1);
	}
	brick.motor('M3').setPower(0);
	while(brick.encoder('E1').read()<=45){
		brick.motor('M1').setPower(90);
		script.wait(1);
	}
	brick.motor('M1').setPower(0);
	brick.motor(S4).setPower(90);
	script.wait(1000);
	brick.motor(S4).setPower(-90);
	script.wait(1000);
	while(brick.encoder('E1').read()>=-45){
		brick.motor('M1').setPower(-90);
		script.wait(1);
	}
	brick.motor('M1').setPower(0);
	while(brick.sensor('A4').read()==0){
	brick.motor('M4').setPower(-100);
	script.wait(1);
	}
}

var setAng =function(osy,a){
	//osy==1 - Left/Right
	//osy!=1 - Up/Down
	//Poluchenie znacheniy s motorov
	if (osy==1) var aold=brick.motor(S5).power();
	else  var aold=brick.motor(S1).power();
	var da=a-aold;
	var step=da/18;
	//Vlevo vpravo discr
	if (osy==1)
	{
		for(var i =1;i<19;i++)
		{
			brick.motor(S5).setPower(aold+step*i);
			brick.motor(S3).setPower(aold+step*i);
			script.wait(1);
		} 
	}
	//vverkh vniz
	else
	{
	
		for(var i =1;i<19;i++)
		{
			brick.motor(S1).setPower(aold+step*i);
			brick.motor(S2).setPower(aold+step*i);
			script.wait(1);
		} 
	}
}

var mess = function(){
	if (mailbox.hasMessages()){
		var mes = mailbox.receive();
	}
		if (mes == 1) {
			brick.motor("M4").setPower(-100);
		} else if (mes ==0) {
			brick.motor("M4").setPower(0);
		}
	}

var main = function(){
	mailbox.connect("10.23.47.113");
	//Timer
	var time0=0, dt=0;
	//Startovoe polozhenie
	setAng(0,0);
	//Ustanovka polozheniya camery
	camera=0;
	brick.motor(S6).setPower(camera);
	//Ugly po osyam
	var aLR=0,aUD=10;
	while(true)
	{
		mess();
		//Schityvanie znacheny
		var Left=brick.sensor(A5).readRawData();
		var Right=brick.sensor(A6).readRawData();
		Left=1023-Left;
		Right=1023-Right;
		var Lsm=toSm(Left);
		var Rsm=toSm(Right);
		//Esly ob'ekt blizhe k odnomu datchiku, chem k drugomu
		if(Math.abs(Lsm-Rsm)>10)
		{
			//Esly sleva
			if(Lsm<Rsm)
			{
				if(aLR<35) aLR+=9;
			}
			//Esly sprava
			else
			{
				if(aLR>-35) aLR-=9;
			}
			time0=-1;
		}
		else
		{
			//esly net
			if(Lsm>=50 && Rsm>=50)
			{
				//esly vpervye
				if(time0==-1)
				{
					time0=script.time();
				}  
				else
				{
					dt=script.time()-time0;
				}
				//vozvrat
				if(dt>5000) {setAng(1,0); aLR=0;} 
			}
			else time0=-1;
		}
		//Dvizhenie L/R
		brick.motor(S5).setPower(aLR);
		brick.motor(S3).setPower(aLR);
		//Raschet ugla U/D po uglu camery
		var per=camera/20;
		var perUp=per*40+10;
		aUD=perUp;
		setAng(0,aUD);
		//Izmenenie ugla camery po nazhatiyu knopky
		if(brick.keys().isPressed(KeysEnum.Up) /*&& camera<20*/)camera++;
		if(brick.keys().isPressed(KeysEnum.Down) /*&& camera>0*/)camera--;
		brick.motor(S6).setPower(camera);
		//Vyvod testovykh peremennykh
		brick.display().addLabel(Lsm,10,10);
		brick.display().addLabel(Rsm,10,25);
		brick.display().addLabel(dt,10,50);
		brick.display().addLabel(camera,10,75);
		brick.display().redraw();
		script.wait(1);
	}
	return;
}