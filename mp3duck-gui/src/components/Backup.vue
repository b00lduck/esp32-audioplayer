<template>

  <b-card class="backup" header="Backup and Restore">

    <b-button-toolbar>
      <b-button-group class="mx-2">
        <b-button v-on:click="doBackup()">create backup archive</b-button>
      </b-button-group>      
    </b-button-toolbar>

  </b-card>
</template>

<script>

  import JSZip from 'jszip'
  import { saveAs } from 'file-saver'

  export default {
    methods: {

      doBackup: async function() {        

        const zip = new JSZip()

        const cardsList = await this.fetchFiles("/cards")

        for (const card of cardsList) {
          if (card.type === "directory") {
            const cardContents = await this.fetchFiles(card.name)                     
            for (const content of cardContents) {              
              const blob = await this.getFile(content.name)
              zip.file(content.name, blob, { createFolders: true })
            }
          }          
        }

        zip.generateAsync({type:"blob"}, f => {console.log(f)}).then(content => {
          saveAs(content, "example-backup.zip");
        })

      },

      getFile: async function(path) {
        return fetch("http://192.168.2.149/api/file" + path, {
          method: "get"
        }).then((res) => {
          if (!res.ok) {
            const error = new Error(res.statusText);
            throw error;
          }
          return res
        })
        .then((data) => {
          return data.blob()
        })
        .catch((err) => {
          this.error = true
          console.log(err)
        })
        .then((data) => {
          this.loading = false
          return data
        });           
      },


      fetchFiles: async function(path) {
        return fetch("http://192.168.2.149/api/file" + path, {
          method: "get"
        }).then((res) => {
            if (!res.ok) {
              const error = new Error(res.statusText);
              throw error;
            }
            return res.json()
          })
          .then((data) => {
            return data
          })
          .catch((err) => {
            this.error = true
            console.log(err)
          })
          .then((data) => {
            this.loading = false
            return data
          });
      }

    },

    data() {
      return {
        loading: false,
        error: false
      }
    },

    name: "Backup",
  }; 
</script>

<style scoped>
  .backup {
    width: 600px; 
  }
</style>
