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
              <playlist-entry @error="onError" v-for="(entry, index) in card.entries" :key="index" :entry="entry" :api="api"/>
            </div>
            <br/>
            <b-button-group style="width: 100%;">                  
              <b-button size="sm" variant="primary" @click="onPickFile" v-bind:disabled="card.state === 'uninitialized'">add playlist entry</b-button>
              <input type="file" style="display: none" accept="audio/*" ref="fileInput" @change="onFilePicked"/>
            </b-button-group>

            <div v-if="uploadProgress.running">
              <CopyProgress v-bind:progress="uploadProgress" />     
            </div>

          </b-col>
        </b-row>                

      </div>
    </div>
  </b-card>
</template>

<script>
import PlaylistEntry from './PlaylistEntry.vue'
import CopyProgress from '../CopyProgress.vue'
import axios from 'axios'

  export default {
  components: { PlaylistEntry, CopyProgress },
    beforeMount() {
      this.interval = setInterval(this.fetchData, 2000);
    },
    methods: {
      onError (err) {
          this.error = err
      },
      onPickFile () {
        this.$refs.fileInput.click()
      },
      onFilePicked (event) {
        this.uploadProgress = {
          overall: {
            totalBytes: 0,
            currentBytes: 0
          },
          currentFile: {
            totalBytes: 0,
            currentBytes: 0,
            name: ""
          },
          canceled: false,
          running: true
        }

        const formData = new FormData()
        formData.append('file', event.target.files[0])
        this.uploadProgress.currentFile.name = event.target.files[0].name

        axios.post(this.api + "/file/cards/" + this.card.id, formData, {
          onUploadProgress: (e) => {
            if (e.type === "progress") {
              this.uploadProgress.currentFile.currentBytes = e.loaded
              this.uploadProgress.currentFile.totalBytes = e.total
              this.uploadProgress.overall.currentBytes = e.loaded
              this.uploadProgress.overall.totalBytes = e.total
            }
          }
        })
        .then((res) => {
          if (res.status !== 201) {
            const error = new Error(res.statusText)
            throw error
          }            
          this.uploadProgress.running = false
        })
        .catch((err) => {
          this.error = err
          this.uploadProgress.running = false
        })   
      },

      initCard: function() {
        return fetch(this.api + "/card/" + this.card.id, {
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

        return fetch(this.api + "/card/current", {
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
              console.log(res)
              return res.json()
            }            
          })
          .then((data) => {
            this.error = null
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
            this.card = null
          })
          .then(() => {
            this.loading = false;            
          });
      }
    },
    data() {
      return {
        card: null,
        loading: false,
        error: false,
        cardName: "",
        uploadProgress: {
          overall: {
            totalBytes: 0,
            currentBytes: 0
          },
          currentFile: {
            totalBytes: 0,
            currentBytes: 0,
            name: ""
          },
          canceled: false,
          running: false
        },
        interval: null
      }
    },
    created() {
      this.fetchData()
    },
    beforeDestroy() {
      clearInterval(this.interval)
    },
    props: ['api'],
    name: "Cardview",
  }; 
</script>

<style scoped>
.card {
}
.row {
  padding-bottom: 10px;
}
label {
  text-align: right;
  width: 100%;
}
</style>
