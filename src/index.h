#include <Arduino.h>     
const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<title>Dublore Metra by.M.Semerkov</title>
<style 
type="text/css">
html, body 
{
  height: 100%;
  width: 100%;
  margin-bottom:0%; 
  margin-top:0%
}

body
{
background: linear-gradient(to top, #000000, #5A5A5A);
}

.border-button 
{
  text-decoration: none;
  display: inline-block;
  padding: 20px 30px;
  margin: 10px 20px;
  position: relative;
  color: white;
  border: 2px solid rgba(255, 255, 255, .6);
  background: none;
  font-weight: 300;
  font-family: 'Montserrat', sans-serif;
  text-transform: uppercase;
  letter-spacing: 2px;
}
.border-button:before, .border-button:after 
{
  content: "";
  position: absolute;
  width: 0;
  height: 0;
  opacity: 0;
  box-sizing: border-box;
}
.border-button:before 
{
  bottom: 0;
  left: 0;
  border-left: 5px solid black;
  border-top: 5px solid black;
  transition: 0s ease opacity .8s, .2s ease width .4s, .2s ease height .6s;
}
.border-button:after 
{
  top: 0;
  right: 0;
  border-right: 5px solid black;
  border-bottom: 5px solid black;
  transition: 0s ease opacity .4s, .2s ease width, .2s ease height .2s;
}
.border-button:hover:before, 
.border-button:hover:after 
{
  height: 100%;
  width: 100%;
  opacity: 1;
}
.border-button:hover:before 
{
  transition: 0s ease opacity 0s, .2s ease height, .2s ease width .2s;
}
.border-button:hover:after
{
  transition: 0s ease opacity .4s, .2s ease height .4s, .2s ease width .6s;
}
.border-button:hover {
  background: rgba(255, 255, 255, .3);
}


</style>
<body><center>
<div>

<p style="font-size:50px; color: black; margin-bottom:0%; margin-top:0%">METRA DUBLORE</p>

<p style="font-size:10vw; color: black; background: linear-gradient(#000000 #000000, #5A5A5A); border: 2px solid white; width: 40%; background: rgba(255, 255, 255, .3); margin-bottom:2%; margin-top:0%" id="adc_val"><b>......</b></p> 

<p style="font-size:60px; color: white; margin-top:0%" id="Stable"><b></b></p>

<a style="font-size:40px; width: 40%;" class="border-button" onclick="send(1)"><b>SET-0</b></a>


</div>

<script>
function send(led_sts) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("state").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "led_set?state="+led_sts, true);
  xhttp.send();
}
setInterval(function() 
{
  get_Weight_Data();
  get_Weight_Stable();
},250); 
function get_Weight_Data() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("adc_val").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "adcread", true);
  xhttp.send();
}
function get_Weight_Stable() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Stable").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "stable", true);
  xhttp.send();
}
</script>
</center>
</body>
</html>
)=====";