var end = 0;
var TASK;
var end = 0; 

TASK = function(){
	while (true){
		while (end != 1){
			end = Threading.receiveMessage(true);
			script.wait(100);
		}
		while(end != 0){
			end = Threading.receiveMessage(true);
			x = script.random(1, 8);
			brick.display().showImage('pict'+x+'.png');
			script.wait(150);
		}
		brick.display().clear();
	}
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
	Threading.sendMessage("speak", 1);//начало работы анимации рта
	var fileName = "\"" + text +".wav\""
	var cmd = "espeak -v russian_test -s 100 -w " + fileName+" \""+ text + "\""
		;
	cmd = "{ { test -r " + fileName + " && echo cached ; } || "
                         + cmd +  " ; } && aplay " + fileName;
	var scriptFilename = "say_text_temp.sh"
	script.system("rm -f " + scriptFilename, true)
	script.writeToFile(scriptFilename, cmd)
	script.system(". ./"+scriptFilename, true)
	Threading.sendMessage("speak", 0);//конец работы анимации рта
}

var main = function(){
	//mailbox.connect(""); //Подключаемся к роботам-помощникам
	//mailbox.connect("");8
	Threading.startThread("speak", "TASK");
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