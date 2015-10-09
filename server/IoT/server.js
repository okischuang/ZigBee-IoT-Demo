
var express = require('express');
var bodyParser = require("body-parser");
var app = express();
var Firebase = require('firebase');


app.use(bodyParser.urlencoded({ extended: false }));

app.get('/',function(req,res){
  res.sendfile("./public/dht.html");
  var temp=req.body.temp;
  console.log("Temperature: = "+temp);
  //res.end("ok");
});

// app.post('/',function(req,res){
//   console.log(req);
//   res.sendfile("./public/dht.html");
//   var temp=req.body.temp;
//   console.log("Temperature: = "+temp);
//   //res.end("ok");
// });

app.post('/iot',function(req,res){
  console.log(req.body);
  var temp=req.body.temp;
  console.log("Temperature: = "+temp);
  res.end("ok");
  var dhtRef = new Firebase("https://webduinodht.firebaseio.com/dht");

  var newDataRef = dhtRef.push();
  var timeStr = new Date().getTime();
  newDataRef.set({
  		timestamp: timeStr,
	    temperature: parseInt(temp)
	});

});

//server.listen(3000, function(){
var port = 3333;
app.listen(port, function(){
        console.log("Server is listening on port ", port);
});

function drawChart(data) {
      data = google.visualization.arrayToDataTable(data);
      chart.draw(data, {
        title: '溫濕度監測',
        hAxis: {
          title: '時間',
          titleTextStyle: {
            color: '#333'
          }
        }
      });
    }
