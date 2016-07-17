var end = 0;
var TASK;
var end = 0; 

var mouth = function(t){
	for(var i=0;i<t;i++){
			x = script.random(1, 8);
			//brick.display().clear();
			brick.display().showImage('pict'+x+'.png');
			//brick.display().redraw();
			script.wait(100);
		}
	//brick.display().clear();
	return;
}

function speak(){
	var lines = [];
	while (brick.keys().isPressed(KeysEnum.Left) != true){
		script.wait(150);
	}
	var command = "cd /home/root/trik/scripts && " +
	" truncate -s 0 input.txt && " +
        " sleep 1.5 && " +
        " aplay ../media/beep.wav && " +
	" arecord -d 3 pew.wav && " +
	" sleep 4 && " + 
	" aplay ../media/beep.wav &&" +
	" timeout -k 2 -s 9 15 curl -v -4 \"asr.yandex.net/asr_xml?key=d7059ba7-65ed-45d2-948e-bef1683250bb&uuid=12345678123456798734567812345678&topic=queries&lang=ru-RU\" -H \"Content-Type: audio/x-wav\" --data-binary \"@pew.wav\" | sed 's/.*confidence=\".*\">//' | sed 's#</.*##' | sed -n \"3p\" > input.txt && " +
	" true "; // <--- last line
	script.system(command, true);
	lines = script.readAll("input.txt");
	return lines[0];
}
function say(text) {
	print ("say:", text)
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
	while (ttxt == 0){
		mouth(2);
		ttxt = script.readAll("text.txt");
	}
	//Threading.sendMessage("speak", 0);//конец работы анимации рта
}

var main = function(){
	//mailbox.connect(""); //Подключаемся к роботам-помощникам
	//mailbox.connect("");8
	//mouth();
	//return;
	//Threading.startThread("speak", "TASK");
	var i = 0;
	var sp_h = script.readAll("sp_human.txt");
	var sp_t = script.readAll("sp_trik.txt");
	say("начали")
	while (true){
		lines = speak();
		switch (lines) {
		case 'начни стирать':
		//mailbox.send(2, ); // Отправляем сигнал на робота-помощника на доске
		break;
		case 'вперед':
			
		break;
		case 'влево':
			
		break;
		case 'вправо':
			
		break;
		case 'назад':
			
		break;
		case 'поворот':
			
		break;
		case 'кругом':
			
		break;
		default: {
		print("lines:<",lines,">")
			for (i = 0; i< sp_h.length; i++){
				if(sp_h[i] == lines){
					break;
				}
			}
			if (i >= sp_h.length){
				say('извините я вас не понял');
			} else {
				say(sp_t[i]);
			}
		} // Просто диалог робота с человеком
		break;
		}
	}
}