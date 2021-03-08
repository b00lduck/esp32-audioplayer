<template>

    <b-card class="card" header="Card info">    
      <div v-if="error">HTTP error<br>{{error}}</div>
      <div v-if="!error">
        <div v-if="!card">no card</div>
        <div v-if="card">

          <b-row>            
            <b-col sm="4">
              <label for="cardId">Card ID:</label>
            </b-col>
            <b-col sm="8">
              <b-form-input size="sm" id="cardId" v-model="card.id" disabled />
            </b-col>
          </b-row>

          <b-row>
            <b-col sm="4">
              <label for="cardName">Card name:</label>
            </b-col>
            <b-col sm="8">              
                <b-button-group style="width: 100%;">
                  <b-form-input size="sm" v-bind:disabled="card.state === 'uninitialized'" v-model="cardName"/>
                  <b-button v-on:click="initCard" size="sm" variant="primary" v-bind:disabled="card.state === 'uninitialized'">rename</b-button>
                  <b-button v-on:click="initCard" size="sm" variant="primary" v-bind:disabled="card.state !== 'uninitialized'">initialize</b-button>      
                </b-button-group>
            </b-col>
          </b-row>

          <b-row>
            <b-col sm="4">
              <label for="cardStatus">Card status:</label>
            </b-col>
            <b-col sm="8">
              <b-form-input size="sm" id="cardStatus" v-model="card.state" disabled />
            </b-col>
          </b-row>                

          <b-row>
            <b-col sm="4">
              <label for="numEntries">Playlist entries:</label>
            </b-col>
            <b-col sm="8">
              <div v-if="card.numEntries">
                <playlist-entry v-for="(entry, index) in card.entries" :key="index" v-bind:entry="entry"/>
              </div>
              <b-button-group style="width: 100%;">                  
                <b-button size="sm" variant="primary" @click="onPickFile" v-bind:disabled="card.state === 'uninitialized'">add entry</b-button>
                <input type="file" style="display: none" accept="audio/*" ref="fileInput" @change="onFilePicked"/>
              </b-button-group>

            </b-col>
          </b-row>                

        </div>
      </div>
    </b-card>
</template>

<script>
import PlaylistEntry from './PlaylistEntry.vue';

  export default {
  components: { PlaylistEntry },
    methods: {

      onPickFile () {
        this.$refs.fileInput.click()
      },
      onFilePicked (event) {
        const files = event.target.files

        const formData = new FormData()
        formData.append('file', files[0])

        return fetch("http://192.168.2.149/api/file/cards/" + this.card.id, {
          method: "post",
          body: formData
        })
        .then((res) => {
          if (!res.ok) {
            const error = new Error(res.statusText);
            throw error;
          }            
        })
        .catch((err) => {
          this.error = err
        })   

      },


      initCard: function() {
        return fetch("http://192.168.2.149/api/card/" + this.card.id, {
          method: "post",
          headers: {
            "content-type": "application/x-www-form-urlencoded"
          },
          body: "name=" + encodeURIComponent(this.cardName)
        })
        .then((res) => {
            if (!res.ok) {
              const error = new Error(res.statusText);
              throw error;
            }            
          })
      },  
      fetchData: function() {
        this.loading = true

        return fetch("http://192.168.2.149/api/card/current", {
          method: "get"
        })
          .then((res) => {
            if (!res.ok) {
              const error = new Error(res.statusText);
              throw error;
            }
            if (res.status === 204) {
              return new Promise((resolve) => {
                resolve(null)
              })
            } else {
              return res.json()
            }            
          })
          .then((data) => {

            if (data === null) {
              this.card = null
            } else {
              if (this.card === null || this.card.id != data.id) {
                this.card = data
                this.cardName = data.name
              }
            }
          })
          .catch((err) => {
            this.error = err
          })
          .then(() => {
            this.loading = false;
            setTimeout(this.fetchData, 2000);
          });
      }
    },
    data() {
      return {
        card: null,
        loading: false,
        error: false,
        cardName: ""
      }
    },
    created() {
      this.fetchData()
    },
    name: "Cardview",
  }; 
</script>

<style scoped>
.card {
  width: 600px;
}
.row {
  padding-bottom: 10px;
}
label {
  text-align: right;
  width: 100%;
}
</style>
