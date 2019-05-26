<template>
  <v-container fluid>
    <v-switch v-model="control.light" label="ไฟถนน"></v-switch>
    <v-switch v-model="tempSiren" @change="openDialog" label="สัญญาณเตือน"></v-switch>

    <v-layout text-xs-center wrap>
      <v-flex xs12>
        <embed id="road-svg" :src="require('../assets/road.svg')" alt="My SVG image" width="300px">
      </v-flex>
    </v-layout>

    <v-dialog v-model="dialog" max-width="290">
      <v-card>
        <v-card-title
          class="headline"
        >คุณต้องการ{{control.siren ? "" : "เ"}}ปิดสัญญาณเตือนใช่หรือไม่</v-card-title>

        <v-card-actions>
          <v-spacer></v-spacer>

          <v-btn color="red darken-1" flat="flat" @click="confirm">ยืนยัน</v-btn>

          <v-btn color="green darken-1" flat="flat" @click="onlyClose">ยกเลิก</v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>
  </v-container>
</template>

<script>
import { db } from "./../plugins/firebase";

export default {
  data: () => ({
    data: {
      road: 0
    },
    control: {
      siren: false,
      light: false
    },
    tempSiren: false,
    dialog: false,
    oldSiren: false
  }),
  mounted() {
    db.ref("data/road").on("value", snapshot => {
      this.data.road = snapshot.val();
    });

    db.ref("control")
      .once("value")
      .then(snapshot => {
        this.control.siren = snapshot.val().siren;
        this.tempSiren = snapshot.val().siren;
        this.control.light = snapshot.val().siren;
      });
  },
  watch: {
    "data.road"(val) {
      if (val < 0) val += Math.pow(2, 32);
      var bit = parseInt(val, 10)
        .toString(2)
        .padStart(32, "0");
      for (let i = 0; i < 32; i++) {
        document.embeds["road-svg"].getSVGDocument().getElementById(`Road-${Math.floor(i / 8)}-${i % 8}`).style.fill =
          bit.charAt(i) === "0" ? "" : "red";
      }
    },
    "control.siren"(val) {
      db.ref("control/siren").set(val);
    },
    "control.light"(val) {
      db.ref("control/light").set(val);
    }
  },
  methods: {
    openDialog() {
      this.dialog = true;
      this.oldSiren = !this.tempSiren;
    },
    onlyClose() {
      this.tempSiren = this.oldSiren;
      this.dialog = false;
    },
    confirm() {
      this.control.siren = !this.control.siren;
      this.dialog = false;
    }
  }
};
</script>
