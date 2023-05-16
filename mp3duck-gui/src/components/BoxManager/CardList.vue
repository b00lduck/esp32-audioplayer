<template>
  <b-card class="cardlist" header="Card list">
    <b-button-toolbar>
      <b-button-group class="mx-2">
        <div>
          <b-button variant="primary" v-on:click="createList()">create card list csv</b-button>
        </div>
      </b-button-group>
    </b-button-toolbar>
  </b-card>
</template>

<script>
import { saveAs } from "file-saver"
import FileAccessService from "../../service/FileAccessService"

const fileAccessService = new FileAccessService()

export default {
  components: {
  },
  methods: {

    createList: async function () {
      
      try {

        const cards = await fileAccessService.getListCards(this.api)
        
        let out = "id;name;numtracks\n"

        // sort cards by id field which is a string
        cards.sort((a, b) => a.id.localeCompare(b.id))

        for (const card of cards) {
          out += card.id + ";" + card.name + ";" + card.numtracks + "\n"
        }
        
        saveAs(new Blob([out], { type: "text/csv;charset=utf-8" }), "cardlist.csv")

      } catch (e) {
        this.error = true
        console.log(e)
        return
      }
    },
  },

  data() {
    return {
      loading: false,
      error: false,
      cardList: [],
     
    }
  },

  props: ["api"],
  name: "Card List",
}
</script>

<style scoped>
.cardList {
}
table {
  font-size: 8px;
}
</style>
