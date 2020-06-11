var testcount=-1;

function testScoreboard() {
	
	if(testcount==-1) {
		// Turn all the LEDs off
		updateScoreboard_Testing('---','-','--','---','---','---')
		var testData = '---' + '-' + '--' + '---' + '---' + '---'; 
		$.ajax({type: "GET",url: "scoreboard.php?data=" + testData,cache: false,success: function () { 
			testcount++;
			setTimeout(function () {testScoreboard();},1000);
		}});
	} else if(testcount==9) {
		// Sets the scores to 9, however, checks if Test Mode has been disabled as well
		updateScoreboard_Testing(testcount + '' + testcount + '' + testcount,'' + testcount + '',testcount + '' + testcount,testcount + '' + testcount + '' + testcount,testcount + '' + testcount + '' + testcount,testcount + '' + testcount + '' + testcount)
		var testData = testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + ''; 
		$.ajax({type: "GET",url: "scoreboard.php?data=" + testData,cache: false,success: function () { 
			if(testmode==1) {
				// If Test Mode is still enabled, set the count to -1 to start again
				testcount=-1;
				setTimeout(function () {testScoreboard();},1000);
			} else {
				// If Test Mode has been disabled, set the count to -1 and update the scores to what they were before Test Mode was enabled
				testcount=-1;
				$('#notification').html('');
				$('#total_count').html(pad(total,3));
				$('#wickets_count').html(pad(wickets,1));
				$('#overs_count').html(pad(overs,2));
				$('#batsa_count').html(pad(batsa,3));
				$('#batsb_count').html(pad(batsb,3));
				$('#target_count').html(pad(target,3));
				submitForm(); // Update the scoreboard
			}
		}});
	} else {
		// Sets the score to the current value of 'testcount' and then adds 1 to this value, wait for 1 second and run again
		updateScoreboard_Testing(testcount + '' + testcount + '' + testcount,'' + testcount + '',testcount + '' + testcount,testcount + '' + testcount + '' + testcount,testcount + '' + testcount + '' + testcount,testcount + '' + testcount + '' + testcount)
		var testData = testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '' + testcount + '';  
		$.ajax({type: "GET",url: "scoreboard.php?data=" + testData,cache: false,success: function () { 
			testcount++;
			setTimeout(function () {testScoreboard();},1000);
		}});
	}
	
}

// This function basically coverts any dashes to greyed out zeros on the interface
function updateScoreboard_Testing(totalt,wicketst,overst,batsat,batsbt,targett) {	
	totalt=totalt.replace(/-/g, "<font color='dddddd'>8</font>"); $('#total_count').html(totalt);
	wicketst=wicketst.replace(/-/g, "<font color='dddddd'>8</font>"); $('#wickets_count').html(wicketst);
	overst=overst.replace(/-/g, "<font color='dddddd'>8</font>"); $('#overs_count').html(overst);
	batsat=batsat.replace(/-/g, "<font color='dddddd'>8</font>"); $('#batsa_count').html(batsat);
	batsbt=batsbt.replace(/-/g, "<font color='dddddd'>8</font>"); $('#batsb_count').html(batsbt);
	targett=targett.replace(/-/g, "<font color='dddddd'>8</font>"); $('#target_count').html(targett);
}

// Enable Test Mode
function admin_testmode() {
	swal({ // Open a popup notification to explain Test Mode
		title: 'Enter Test Mode',
		html: "Test Mode will keep cycling through a series of light configurations until it is disabled. The scoreboard will not display scores while in Test Mode.<br/><br/><b>When Test Mode has ended, the previous scores will be displayed.</b>",
		type: 'warning',
		showCancelButton: true,
		confirmButtonColor: '#3085d6',
		cancelButtonColor: '#d33',
		confirmButtonText: 'Enter Test Mode',
		closeOnConfirm: false
	}).then(function(isConfirm) {
		if (isConfirm) {
			swal({ // Prompt user for admin password
			title: 'AUTHORISATION REQUIRED',
			html: '<p><input id="test_mode_auth" class="form-control" placeholder="Admin Password" type="password">',
			type: 'error',
			showCancelButton: true,
			closeOnConfirm: true,
			allowEscapeKey: false,
			confirmButtonText: 'Verify',
			allowOutsideClick: false
		}).then(function(isConfirm) {
			if (isConfirm) { // Check if password is correct, if it is enable Test Mode. If not, show notification to say failed
				$.get("verifyadmin.php?pass=" + $('#test_mode_auth').val(), function(data, status){
					if(data=="good") {
						testmode=1;
						testScoreboard();
						$('#notification').html('<div class="alert alert-warning" role="alert"><b>Test Mode Enabled</b> Test mode will keep cycling until it is stopped. <a href="javascript:disableTestMode();">Click here to stop</a></div>');
					} else {
						swal({
							title: 'AUTHORISATION FAILED',
							text: 'Password entered incorrectly. Action not completed.',
							type: 'error',
							closeOnConfirm: true,
							allowEscapeKey: false,
							confirmButtonText: 'OK',
							allowOutsideClick: false
						})
					}
				});
			}
		});
	}
})
}

// Disables Test Mode
function disableTestMode() {
	testmode=0;
	$('#notification').html('<div class="alert alert-info" role="alert"><b>Test Mode Disabled</b> Test mode will stop at the end of this cycle.');
}