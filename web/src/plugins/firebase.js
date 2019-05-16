import "firebase/database";

import * as firebase from "firebase/app";

// Your web app's Firebase configuration
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

// Get a reference to the database service
var db = firebase.database();

export default firebase;
export { db };
