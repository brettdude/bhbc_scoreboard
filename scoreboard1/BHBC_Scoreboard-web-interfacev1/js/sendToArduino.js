		// Simple function to submit the current score values to the PHP script that updates the Arduino
		function submitForm(){
			var formData2 = padToSend(ScoreVisitor, 2) + padToSend(Innings, 2) + padToSend(ScoreHome, 2) + padToSend(Balls, 1) + padToSend(Strikes, 1) + padToSend(Outs, 1); 

            $.ajax({
                type: "GET",
                url: "scoreboard.php?data=" + formData2,
                cache: false,
                success: function () {},
                error: function () {}
            });
        }