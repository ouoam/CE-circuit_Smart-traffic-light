import Vue from "vue";
import Vuex from "vuex";
import { db } from "./plugins/firebase";

Vue.use(Vuex);

var testRef = db.ref("/");

testRef.once("value").then(function(snapshot) {
  console.log(snapshot.val());
});
var old = 0;
var sum = 0;
var count = 0;
testRef.on("value", function(snapshot) {
  let val = snapshot.val().road[0];
  let ch = val - old;
  if (ch <= 0) {
    sum = 0;
    count = 0;
  } else {
    sum += ch;
    count++;
  }
  console.log("old:", old, "to:", val, "change:", ch, "avg:", sum / count, sum, count);
  old = val;
});

export default new Vuex.Store({
  state: {},
  mutations: {},
  actions: {}
});
