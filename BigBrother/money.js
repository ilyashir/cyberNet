var time0=script.time();
var main = function()
{
	while(true)
	{
		while (!brick.keys().isPressed(KeysEnum.Up) || (script.time()-time0)<5000)script.wait(1);
		brick.encoder(E1).reset();
		brick.motor(M1).setPower(100);
		while(brick.encoder(E1).read()<40) script.wait(1);
		brick.motor(M1).setPower(0);
		while ( brick.keys().isPressed(KeysEnum.Up))script.wait(1);
		time0=script.time();
	}
	return;
}
