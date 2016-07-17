var end = 0;
var TASK;
var end = 0; 

TASK = function(){
	while (true){
		while (end != 1){
			end = Threading.receiveMessage(true);
			script.wait(50);
		}
		while(end != 0){
			x = script.random(1, 8);
			if(x == 1){
				brick.display().showImage('pict1.png');
			}
			if(x == 2){
				brick.display().showImage('pict2.png');
			}
			if(x == 3){
				brick.display().showImage('pict3.png');
			}
			if(x == 4){
				brick.display().showImage('pict4.png');
			}
			if(x == 5){
				brick.display().showImage('pict5.png');
			}
			if(x == 6){
				brick.display().showImage('pict6.png');
			}
			if(x == 7){
				brick.display().showImage('pict7.png');
			}
			if(x == 8){
				brick.display().showImage('pict8.png');
			}
			script.wait(100);
			end = Threading.receiveMessage(true);
		}
		brick.display().clear();
	}
	return;
}

function speak(){
	var lines = [];
	script.system("cd scripts")
	script.removeFile("input.txt");
	script.writeToFile("input.txt", "");
	while (brick.keys().isPressed(KeysEnum.Left) != true){
		script.wait(50);
	}
	script.wait(1500);
	brick.playSound("media/beep.wav");
	script.system("arecord -d 3 pew.wav");
	script.wait(4000);
	brick.playSound("media/beep.wav");
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
	//script.system('cmd', true);
	lines = script.readAll("input.txt");
	return lines[0];
}

var main = function(){
	//mailbox.connect(""); //Подключаемся к роботам-помощникам
	//mailbox.connect("");8
	Threading.startThread("speak", "TASK");
	var i = 0;
	var sp_h = script.readAll("sp_human.txt");
	var sp_t = script.readAll("sp_trik.txt");
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
			for (i = 0; sp_h[i] != lines; i++){
				if (i > 15){
					break;
				}
				script.wait(10);
			}
			if (i > 15){
				Threading.sendMessage("speak", 1);//начало работы анимации рта
				brick.say('извините я вас не понял');
				script.system("cmd", true);
				///script.wait(5000);
				Threading.sendMessage("speak", 0);//конец работы анимации рта
			} else {
				Threading.sendMessage("speak", 1);//начало работы анимации рта
				brick.say(sp_t[i]);
				script.system("cmd", true);
				//script.wait(5000);
				Threading.sendMessage("speak", 0);//конец работы анимации рта
			}
		} // Просто диалог робота с человеком
		break;
		}
	}
}