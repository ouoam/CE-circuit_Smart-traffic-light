import Vue from "vue";
import Vuex from "vuex";
import { db } from "./plugins/firebase";

Vue.use(Vuex);

var starCountRef = db.ref("data/road");
starCountRef.on("value", function(snapshot) {
  console.log(snapshot.val(), parseInt(snapshot.val(), 10).toString(2));
});

export default new Vuex.Store({
  state: {},
  mutations: {},
  actions: {}
});
