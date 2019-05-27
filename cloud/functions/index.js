const functions = require("firebase-functions");
const firebase = require("firebase/app");
const request = require("request-promise");

require("firebase/database");

var firebaseConfig = {
  apiKey: "",
  authDomain: "smart-traffic-light-ec485.firebaseapp.com",
  databaseURL: "https://smart-traffic-light-ec485.firebaseio.com",
  projectId: "smart-traffic-light-ec485",
  storageBucket: "smart-traffic-light-ec485.appspot.com",
  messagingSenderId: "590491732196",
  appId: "1:590491732196:web:51470f3bb1bb2cfc"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);

// // Create and Deploy Your First Cloud Functions
// // https://firebase.google.com/docs/functions/write-firebase-functions
//
// exports.helloWorld = functions.https.onRequest((request, response) => {
//  response.send("Hello from Firebase!");
// });

exports.makeUppercase = functions.database.ref("/data/road").onUpdate(snapshot => {
  const val = snapshot.after.val();
  var bit = parseInt(val, 10)
    .toString(2)
    .padStart(32, "0");

  var dir = [0, 0, 0, 0];
  for (let i = 0; i < 32; i++) {
    if (bit.charAt(i) === "1") {
      dir[Math.floor(i / 8)]++;
    }
  }

  firebase
    .database()
    .ref("time")
    .once("value")
    .then(snapshot => {
      var time = [0, 0, 0, 0];
      time["0"] = snapshot.val()["0"] ? snapshot.val()["0"] : time["0"];
      time["1"] = snapshot.val()["1"] ? snapshot.val()["1"] : time["1"];
      time["2"] = snapshot.val()["2"] ? snapshot.val()["2"] : time["2"];
      time["3"] = snapshot.val()["3"] ? snapshot.val()["3"] : time["3"];

      for (var i = 0; i < 4; i++) {
        if (dir[i] >= 5 && time[i] + 10000 < Date.now()) {
          notifyLine("ขณะนี้การจรจารฝั่งขาเข้าสี่แยกทางทิศ" + ["เหนือ", "ตะวันตก", "ใต้", "ตะวันออก"][i] + "หนาแน่น");
          firebase
            .database()
            .ref("time/" + i)
            .set(Date.now());
        }
      }

      return true;
    })
    .catch();
});

const notifyLine = (lineMessage, stkPkgId, stkId) => {
  return request({
    method: `POST`,
    uri: `https://notify-api.line.me/api/notify`,
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
      Authorization: `Bearer O5UbGYgOBqPVuLjFs1CYUDW8Bek7a37VKqx8jDFbVE7`
    },
    form: {
      message: lineMessage,
      stickerPackageId: stkPkgId,
      stickerId: stkId
    }
  });
};
