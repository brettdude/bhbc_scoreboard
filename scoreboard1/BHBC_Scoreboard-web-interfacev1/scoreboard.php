<?php
	//
	// 20.01.2017
	//
	// Customized for Baseball scoreboard 
	// Based on the great https://buildyourownscoreboard.wordpress.com/
	// Tweaked for the Namur Angels Baseball and Softball club - Belgium
	// 
	// 
	
	file_put_contents('save.txt', $_GET['data']); // Save the scores! They will be automatically loaded when the scoreboard is turned on. Make sure save.txt is writable by the web server user!
	
	include "php_serial.class.php";

	$ScoreHome=substr($_GET['data'], 0, 2); //Get first 2 digits for score Homes
	$Topbot=substr($_GET['data'], 2, 1); //Get the next digit to indicate Top or Bottom of innings
	$Innings=substr($_GET['data'], 3, 2); //Get next 2 digits: that is the Inning
	$ScoreVisitor=substr($_GET['data'], 5, 2); //Get next 2 digits: the Score for the Namur Angels (Visitor)
	$Balls=substr($_GET['data'], 7, 1); //Get next 1 digit: that is the Balls (max 3)
	$Strikes=substr($_GET['data'], 8, 1); //Get next 1 digit: capture the value for the strikes (max 2)
	$Outs=substr($_GET['data'], 9, 1); //Get next 1 digit: the number of Outs (max 2)
	
	// Now we want to check if the mutiple digit displays are under 10 or over 10 and turn off the LED's instead of showing 0
	// But if they are 0, show 0
	//
	// First strip the leading zeros, then "pad" the number to the required length by adding dashes to turn off
	// LEDs that would be showing 0 before the actual score
	
	if($ScoreHome==0) { $ScoreHome="-0"; } else { $ScoreHome = ltrim($ScoreHome, '0'); $ScoreHome = str_pad($ScoreHome, 2, '-', STR_PAD_LEFT); }
	if($ScoreVisitor==0) { $ScoreVisitor="-0"; } else { $ScoreVisitor = ltrim($ScoreVisitor, '0'); $ScoreVisitor = str_pad($ScoreVisitor, 2, '-', STR_PAD_LEFT); }
	if($Innings==0) { $Innings="-0"; } else { $Innings = ltrim($Innings, '0'); $Innings = str_pad($Innings, 2, '-', STR_PAD_LEFT); }
	if($Strikes==1) { $Strikes=7; }
	if($Strikes==2) { $Strikes=8; }
	if($Outs==1) { $Outs=7; }
	if($Outs==2) { $Outs=8; }
	if($Topbot==0) { $Topbot=7; } else { $Topbot = L; }
	
	// Code to turn the Balls value into 3-bar LED "dots"
	// Dot 1 will be connected to pins for A, B, and C, and Dot 2 will connect to D, E, and F.
	// Dot 3 will connect to any 3 pins on the next register.

	switch ($Balls) {
		case 0:
			$Balls1=0;
			$Balls2=0;
		break;
		case 1:
			$Balls1=7;
			$Balls2=0;
		break;
		case 2:
			$Balls1=8;
			$Balls2=0;
		break;
		case 3:
			$Balls1=8;
			$Balls2=8;
		break;
	}
	
	$serial = new phpSerial;
	$serial->deviceSet("/dev/ttyACM0");
	#$serial->deviceSet("/dev/ttyAMA0");
	#$serial->confBaudRate(57600);
	$serial->confParity("none");
	$serial->confCharacterLength(8);
	$serial->confStopBits(1);
  
  
  
  
	//uncomment the following line is you want the second line of the scoreboard to be over, wickets target
	//$tempString="4,".$batsmana.",".$total.",".$batsmanb.",".$overs.",".$wicketsOnes.",".$target."#";
	$tempString="4,".$ScoreHome.",".$Topbot.",".$Innings.",".$ScoreVisitor.",".$Balls1.",".$Balls2.",".$Strikes.",".$Outs."#";
	echo $tempString;
	#echo($tempString."<br>");
	$serial->deviceOpen();
	$serial->sendMessage($tempString);


	$serial->deviceClose();

	#print_r($_POST);
?>
