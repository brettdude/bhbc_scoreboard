/**
####################
# The scoreboard interface for Bradford on Avon Cricket Club is accessed via a remote laptop running a Linux based operating system which automatically launches into a web
# browser in full screen mode. Problem is, the user would be unable to see the battery power status. Thanks to the HTML5 Battery API, it is possible to get the laptop's 
# battery status and display it on the interface.
####################
*/

		// Initialise Variables
		var battery;

		// Define functions

		/**
		 * Successful callback providing a Battery Manager object.
		 * @param batteryManager
		 */
		function batterySuccess(batteryManager) {
			// Assign batteryManager to globally 
			//   available `battery` variable.
			battery = batteryManager;

			// Register event handlers  
			battery.addEventListener('chargingchange', updatedBatteryStats);
			battery.addEventListener('chargingtimechange', updatedBatteryStats);
			battery.addEventListener('dischargingtimechange', updatedBatteryStats);
			battery.addEventListener('levelchange', updatedBatteryStats);

			// Process updated data
			updatedBatteryStats();
		}

		/**
		 * Failure callback.
		 */
		function batteryFailure() {
			// Fail gracefully. 
		}

		/**
		 * Update HTML with current battery stats
		 */
		function updatedBatteryStats() {
			// Example data in window.battery:
			//   BatteryManager
			//     charging: false
			//     chargingTime: Infinity
			//     dischargingTime: 12600
			//     level: 0.56
			//     onchargingchange: null
			//     onchargingtimechange: null
			//     ondischargingtimechange: null
			//     onlevelchange: null
			if(window.battery.charging==false) { 
				var level=window.battery.level*100; level=level.toFixed(0);
				$('#battinfo').html(level + '%');
					if(window.battery.charging==true) { battwilldie=0; }
					if(level==2) {
						if(battwilldie==0) {
							battwilldie=1;
							swal({
							  title: 'BATTERY LEVEL CRITICAL',
							  html: 'The battery power for the controller is critically low! And will power down very soon. Attach to power to prevent scoreboard failure!',
							  type: 'warning',
							  closeOnConfirm: true,
							  allowEscapeKey: false,
							  confirmButtonText: 'OK',
							  allowOutsideClick: false
							})
						}
					}
					if(level>75) {
						$('#battinfo').append(' <i class="fa fa-battery-full" aria-hidden="true"></i>');
					} else if(level>65) {
						$('#battinfo').append(' <i class="fa fa-battery-three-quarters" aria-hidden="true"></i>');
					} else if(level>35) {
						$('#battinfo').append(' <i class="fa fa-battery-half" aria-hidden="true"></i>');
					} else if(level>10) {
						$('#battinfo').append(' <i class="fa fa-battery-quarter" style="color:#FF0000;" aria-hidden="true"></i>');
					} else if(level<11) {
						$('#battinfo').append(' <i class="fa fa-battery-empty faa-flash animated" style="color:#FF0000;" aria-hidden="true"></i>');
					}
					if(level==20) {
						swal({
							  title: 'BATTERY POWER',
							  html: 'The battery power for the controller is getting low! Attach to power to prevent scoreboard failure.<br/><br/>You have approximately ' + window.battery.dischargingTime + ' seconds of charge left.',
							  type: 'warning',
							  closeOnConfirm: true,
							  allowEscapeKey: false,
							  confirmButtonText: 'OK',
							  allowOutsideClick: false
							})
					}
					if(level==10) {
						swal({
							  title: 'BATTERY POWER',
							  html: 'The battery power for the controller is critically low! Attach to power to prevent scoreboard failure.<br/><br/>You have approximately ' + window.battery.dischargingTime + ' seconds of charge left.',
							  type: 'warning',
							  closeOnConfirm: true,
							  allowEscapeKey: false,
							  confirmButtonText: 'OK',
							  allowOutsideClick: false
							})
					}
			} else {
				$('#battinfo').html('');
			}
		}

		if("getBattery" in navigator) {
			// API is supported

			// Request battery manager object.
			navigator.getBattery().then(batterySuccess, batteryFailure);

		} else {
			// API is not supported, fail gracefully.
		}