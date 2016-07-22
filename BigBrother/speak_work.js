var pi = 3.1415926535897931;
var end = 0;
var u1, u2, v = 90, k1 = 10, k2 = 10, r=4, rr;
var i = 0;
var sp_h = script.readAll("sp_human.txt");
var sp_t = script.readAll("sp_trik.txt");

var p_reg = function(){
	var a=brick.sensor("A4").readRawData();
	var b=brick.sensor("A5").readRawData();
	u1=(Math.min(a,b)-800)/k1;
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
	while (Math.min(Math.min(a,b),Math.min(c,d))/180.0*pi*4/Math.sqrt(2.0)<dist && light < 800){
		if(v>0)
			light = brick.sensor("A1").readRawData();
		else
			light = brick.sensor("A2").readRawData();
		if (brick.sensor("A3").readRawData() < 200){
			p_reg();
		} else {
			end = 1;
			break;
		}
		a=Math.abs(brick.encoder("E1").read());
		b=Math.abs(brick.encoder("E2").read());
		c=Math.abs(brick.encoder("E3").read());
		d=Math.abs(brick.encoder("E4").read());
		/*print("A1 = " + brick.sensor("A1").readRawData());
		print("A2 = " + brick.sensor("A2").readRawData());
		print("A4 = " + brick.sensor("A4").readRawData());*/
	}
	brick.motor("M1").powerOff();
	brick.motor("M2").powerOff();
	brick.motor("M3").powerOff();
	brick.motor("M4").powerOff();
}

var mouth = function(t){
	for(var i=0;i<t;i++){
		x = script.random(1, 8);
		brick.display().showImage('pict'+x+'.png');
		script.wait(100);
	}
}

var pereb = function(){
	for (i = 0; sp_h[i] != lines; i++){
		if (i > 51){
			break;
		}
		script.wait(10);
	}
	if (i > 51){
		say('извините я вас не понял');
	} else {
		say(sp_t[i]);
	}
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
	light = brick.sensor("A6").readRawData();
	while (Math.min(Math.min(a,b),Math.min(c,d))/180.0*pi*4/Math.sqrt(2.0)<dist && light < 800){
		light = brick.sensor("A6").readRawData();
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

function speak(){
	var lines = [];
	script.system("cd scripts")
	script.removeFile("input.txt");
	script.writeToFile("input.txt", "");
		brick.playSound("media/beep.wav");
		script.system("arecord -d 3 pew.wav");
		script.wait(4000);
		//brick.playSound("pew.wav");
		script.system("curl -v -4 \"asr.yandex.net/asr_xml?key=d7059ba7-65ed-45d2-948e-bef1683250bb&uuid=12345678123456798734567812345678&topic=queries&lang=ru-RU\" -H \"Content-Type: audio/x-wav\" --data-binary \"@pew.wav\" | sed 's/.*confidence=\".*\">//' | sed 's#</.*##' | sed -n \"3p\" > input.txt");
		var t = Date.now();
		var dt = Date.now();
		var r = 0;
		while (lines == 0 && r < 15000){
			lines = script.readAll("input.txt");
			dt = Date.now();
			script.wait(10);
			r = dt - t;
		}
	lines = script.readAll("input.txt");
	script.wait(50);
	return lines[0];
}

var Helper = function(){
		while (brick.sensor("A4").readRawData() < 1023 && brick.sensor("A5").readRawData() < 1023){
		brick.motor("M1").setPower(-100);
		brick.motor("M2").setPower(-100);
		brick.motor("M3").setPower(-100);
		brick.motor("M4").setPower(-100);
	}
	v = -90;
	go_on_edge(1000);
	v = 90;
	go_on_edge(5);
	var t = script.time();
	while (script.time() - t < 500){
		brick.motor("M1").setPower(100);
		brick.motor("M2").setPower(100);
		brick.motor("M3").setPower(100);
		brick.motor("M4").setPower(100);
	}
	brick.motor("M1").setPower(0);
	brick.motor("M2").setPower(0);
	brick.motor("M3").setPower(0);
	brick.motor("M4").setPower(0);
	v = -90;
	rotate(90);
	while (brick.sensor("A6").readRawData() < 1023){
		brick.motor("M1").setPower(100);
		brick.motor("M2").setPower(100);
		brick.motor("M3").setPower(100);
		brick.motor("M4").setPower(100);
	}
	brick.motor("M1").setPower(0);
	brick.motor("M2").setPower(0);
	brick.motor("M3").setPower(0);
	brick.motor("M4").setPower(0);
	script.wait(1000);
	brick.motor("S1").setPower(80);
	var mes = 4;
	var x = 0;
	while (x != 4){
		if (gamepad.isPadPressed(1)) {
			x = [gamepad.padX(0), gamepad.padY(0)][0];
		}
		script.sendMessage(mes);
		script.wait(100);
	}
	mes = 7;
	script.wait(1000);
	while (x != 9){
		if (gamepad.isPadPressed(1)){
			x = [gamepad.padX(0), gamepad.padY(0)][0];
		}
		if (brick.keys().isPressed(KeysEnum.Down) == true){
			 while (x != 7){
				if (gamepad.isPadPressed(1)) {
					x = [gamepad.padX(0), gamepad.padY(0)][0];
				}
				script.sendMessage(mes);
				script.wait(100);
			}
		}
	}
	script.wait(1000);
	brick.motor("S1").setPower(80);
	t = script.time();
	while (script.time() - t < 500){
		brick.motor("M1").setPower(-100);
		brick.motor("M2").setPower(-100);
		brick.motor("M3").setPower(-100);
		brick.motor("M4").setPower(-100);
	}
	brick.motor("M1").setPower(0);
	brick.motor("M2").setPower(0);
	brick.motor("M3").setPower(0);
	brick.motor("M4").setPower(0);
	v = 90;
	rotate(90);
	while (brick.sensor("A4").readRawData() < 1023 && brick.sensor("A5").readRawData() < 1023){
		brick.motor("M1").setPower(-100);
		brick.motor("M2").setPower(-100);
		brick.motor("M3").setPower(-100);
		brick.motor("M4").setPower(-100);
	}
	go_on_edge(20);
	while (brick.sensor("A6").readRawData() < 1023){
		brick.motor("M1").setPower(100);
		brick.motor("M2").setPower(100);
		brick.motor("M3").setPower(100);
		brick.motor("M4").setPower(100);
	}
	brick.motor("M1").setPower(0);
	brick.motor("M2").setPower(0);
	brick.motor("M3").setPower(0);
	brick.motor("M4").setPower(0);
}

var do_ = function(){
	switch (lines) {
		case "доска\n":
			Helper();
		break;
		case "возьми\n":
			mailbox.send(2,2);
		break;
		case "отдай\n":
			mailbox.send(2,3);
		break;
		default:
			pereb();
		break;
	}
}

function say(text) {
	var fileName = "\"" + text +".wav\""
	script.system("rm -f text.txt",true);
	var cmd = "espeak -v russian_test -s 100 -w " + fileName+" \""+ text + "\""
		;
	cmd = "{ { test -r " + fileName + " && echo cached ; } || "
                         + cmd +  " ; } ";
	
	var scriptFilename = "say_text_temp.sh";
	script.system("rm -f " + scriptFilename, true);
	script.writeToFile(scriptFilename, cmd);
	script.system(". ./"+scriptFilename,true);
	script.system("aplay " + fileName + " && echo 123 > text.txt");
	var ttxt = script.readAll("text.txt");
	script.wait(1000);
	mailbox.send(2,1);
	while (ttxt == 0){
		mouth(2);
		ttxt = script.readAll("text.txt");
	}
	mailbox.send(2,0);
}

var first = function(){
	script.wait(1000);
	while (brick.sensor("A4").readRawData() < 1023 && brick.sensor("A5").readRawData() < 1023){
		brick.motor("M1").setPower(-100);
		brick.motor("M2").setPower(-100);
		brick.motor("M3").setPower(-100);
		brick.motor("M4").setPower(-100);
	}
	while (true){
		print(brick.sensor("A4").readRawData());
		go_on_edge(1000);
		script.wait(300);
		v = -90;
		if (end == 1){
			break;
		}
		go_on_edge(1000);
		v = 90;
		if (end == 1){
			break;
		}
	}
	end = 0;
	brick.motor("M1").setPower(0);
	brick.motor("M2").setPower(0);
	brick.motor("M3").setPower(0);
	brick.motor("M4").setPower(0);
}

var second = function(){
	script.wait(1000);
	while (brick.sensor("A6").readRawData() < 1023){
		brick.motor("M1").setPower(100);
		brick.motor("M2").setPower(100);
		brick.motor("M3").setPower(100);
		brick.motor("M4").setPower(100);
	}
	brick.motor("M1").setPower(0);
	brick.motor("M2").setPower(0);
	brick.motor("M3").setPower(0);
	brick.motor("M4").setPower(0);
	while (brick.sensor("A3").readRawData() < 200){
		if (brick.keys().isPressed(KeysEnum.Left) == true){
			lines = speak();
			do_();
		}
		if (brick.keys().isPressed(KeysEnum.Right) == true){
			say("меня зовут робот старший брат");
			mailbox.send(2,4);
		}
		script.wait(100);
	}
}

var main = function(){
	x = script.random(1, 8);
	brick.display().showImage('pict'+x+'.png');
	mailbox.connect("10.23.47.211");//камера
	script.wait(1000);
	while (true){
		second();
		script.wait(1000);
		first();
		script.wait(1000);
	}
	return;
}