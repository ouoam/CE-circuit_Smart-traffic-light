<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12>
        <embed id="road-svg" :src="require('../assets/road.svg')" alt="My SVG image" width="100%">
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
import { db } from "./../plugins/firebase";

export default {
  data: () => ({
    data: {
      road: 0
    }
  }),
  mounted() {
    db.ref("data/road").on("value", snapshot => {
      this.data.road = snapshot.val();
    });
  },
  watch: {
    "data.road"(val) {
      if (val < 0) val += Math.pow(2, 32);
      var bit = parseInt(val, 10)
        .toString(2)
        .padStart(32, "0");
      for (let i = 0; i < 32; i++) {
        document.embeds["road-svg"]
          .getSVGDocument()
          .getElementById(`Road-${Math.floor(i / 8)}-${i % 8}`).style.fill =
          bit.charAt(i) === "0" ? "" : "red";
      }
    }
  }
};
</script>
