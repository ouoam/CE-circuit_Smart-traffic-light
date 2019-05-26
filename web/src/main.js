import "./plugins/vuetify";
import "./plugins/firebase";

import App from "./App.vue";
import Vue from "vue";

Vue.config.productionTip = false;

new Vue({
  render: h => h(App)
}).$mount("#app");
