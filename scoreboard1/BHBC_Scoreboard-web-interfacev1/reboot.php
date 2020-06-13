<?php 
/*
    WARNING
	This method requires www-data user on the Raspberry Pi to be given access to the /sbin/shutdown command.
	This can be acheived by editing the /etc/sudoers (as root) to include the following line:
	
	www-data ALL = NOPASSWD: /sbin/shutdown
	
	Due to the nature of this project (running on a secure private network) there shouldn't be many security concerns using this method - worst thing that can happen is the scoreboard is shutdown. However, you should note that anyone who can access the web interface will be able to shutdown/restart the scoreboard.
*/
shell_exec('sudo /sbin/shutdown -r now'); 
?>