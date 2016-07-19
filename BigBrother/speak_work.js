var pi = 3.1415926535897931;
var end = 0, v = 90; 

var mouth = function(t){
	for(var i=0;i<t;i++){
		x = script.random(1, 8);
		brick.display().showImage('pict'+x+'.png');
		script.wait(100);
	}
}

var pereb(){
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
	while (brick.keys().isPressed(KeysEnum.Left) != true){
		script.wait(50);
	}
	brick.playSound("media/beep.wav");
	script.system("arecord -d 3 pew.wav");
	script.wait(4000);
	brick.playSound("pew.wav");
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
	return lines[0];
}

var do_(){
	switch (lines) {
		case "начни стирать\n":
			go(2);
			rotate(180);
			var mes=1; 
			script.sendMessage(mes);
			script.wait(1000);
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

var main = function(){
	mailbox.connect("10.23.46.33");
	var i = 0;
	var sp_h = script.readAll("sp_human.txt");
	var sp_t = script.readAll("sp_trik.txt");
	while (true){
		lines = speak();
		do_();
	}
	return;
}
