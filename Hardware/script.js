var five = require("johnny-five"),
    board = new five.Board();
var firebase = require("firebase");

var config = {
    apiKey: "AIzaSyDf4dwlikPZ4RfOgiDjQIN5HCeAqc7BWEw",
    authDomain: "meuprimeiroprojeto-a1c81.firebaseapp.com",
    databaseURL: "https://meuprimeiroprojeto-a1c81.firebaseio.com/",
    storageBucket: "meuprimeiroprojeto-a1c81.appspot.com",
};
firebase.initializeApp(config);

function writeEnergy(userId, v, a, w) {
    firebase.database().ref('Sensor/' + userId).set({
        u: v,
        i: a,
        p: w
    });
}

board.on("ready", function(){
    // Create an Led on PWM pin 6
    var volt, c_value,
        led = new five.Led(6),
        tc5v = new five.Sensor({
            pin:"A0",
            freq:"1000",
            threshold: 2
        });
    this.repl.inject({
        // Allow limited on/off control access to the
        // Led instance from the REPL.
        led: led
    });
    tc5v.on("data", function(){
        var data = this.fscaleTo([0, 100])-100;
        data = data<0? Math.abs(data):data;
        volt = ((5.0 / 1023.0) * data);
        c_value = volt / 0.04;
        var pot = volt*c_value;
        writeEnergy("SCT0101T-D", volt, c_value, pot);
    //  console.log(`Sensor: ${data} | I: ${c_value}A | U = ${volt}V`);
    });
});