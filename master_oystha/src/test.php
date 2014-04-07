<html>
<head>
<title>TEST</title>
<style type="text/css">
	p { display : table.cell; }
	button { width: 75px, margin 2px auto; }
</style>
<?php
function writeCommand($somecontent){
	$filename = '/home/ubuntu/commands.txt';
	if(is_writable($filename)) {
		if(!$fp = fopen($filename, 'w')) {
			echo "Cannot open file ($filename)";
			exit;
		}
		if(fwrite($fp, $somecontent) === FALSE) {
			echo "Cannot write to file ($filename)";
			exit;
		}
		//echo "Success, wrote ($somecontent) to file ($filename)";
		fclose($fp);
	} else {
		echo "The file $filename is not writable";
	}
}
/*
$cmd = $_GET['cmd'];
echo "command";
echo $cmd;
switch($cmd){
	case PLAY:
		writeCommand("PLAY\n");
		break;
	default:
		writeCommand("STOP\n");
}
*/
if(isset($_GET['cmd'])){
	writeCommand($_GET['cmd']."\n");
	unset($_GET['cmd']);
}
?>
</head>
<body>
<div style="width: 200px; margin: 0px auto;">
	<div style="width: 100px; float left;">
		<p>BUTTONS</p>
		<button type="button" onclick="location.href='test.php?cmd=cam_play'">PLAY</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_stop'">STOP</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_record'">REC</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_rew'">REW</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_fwd'">FWD</button>
		<button type="button" onclick="location.href='test.php?cmd=elm_sendPulse'">ON</button>
		<button type="button" onclick="location.href='test.php?cmd=EXIT'">EXIT</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_menu'">MENU</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_menuUp'">UP</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_menuDown'">DOWN</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_menuRight'">RIGHT</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_menuLeft'">LEFT</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_execute'">ENTER</button>
		<button type="button" onclick="location.href='test.php?cmd=cam_photoCapture'">PHOTO</button>
	<div>
</div>
</body>
</html>
