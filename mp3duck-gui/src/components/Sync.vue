<template>
  <b-card class="sync" header="Sync two boxes">
    
    <div v-if="error">HTTP error<br>{{error}}</div>
    <div v-if="listCreated">
      <CopyProgress v-bind:progress="progress" />
      <FileListTable v-bind:fileList="syncFileList" />
      <br />      
    </div>

    <b-button-toolbar>
      <b-button-group class="mx-2">
        <div v-if="!progress.running">
          <b-button variant="primary" v-on:click="createList()">(re)create sync list</b-button>
          <b-button variant="primary" v-on:click="sync()">start sync</b-button>
        </div>
        <div v-if="progress.running">
          <b-button variant="danger" v-on:click="cancel()">abort sync</b-button>
        </div>
      </b-button-group>
    </b-button-toolbar>
  </b-card>
</template>

<script>
import CopyProgress from "./CopyProgress";
import FileListTable from "./FileListTable";
import FileAccessService from "../service/FileAccessService";
import axios from 'axios'

const fileAccessService = new FileAccessService()

export default {
  components: {
    CopyProgress,
    FileListTable
  },
  methods: {

    cancel: async function () {
      this.progress.canceled = true
    },

    createList: async function () {

      this.progress = {
        overall: {
          totalBytes: 0,
          currentBytes: 0
        },
        currentFile: {
          totalBytes: 0,
          currentBytes: 0,
          name: ""
        },
        count: {
          totalFiles: 0,
          currentFiles: 0
        },
        canceled: false,
        running: true
      }
       
      try {
        this.listCreated = false
        let { fileList, totalBytes, totalFiles } = await fileAccessService.getSyncList(this.api1, this.api2)        
        this.progress.overall.totalBytes = totalBytes
        this.progress.count.totalFiles = totalFiles
        this.syncFileList = fileList
        this.listCreated = true
      } catch (e) {
        this.error = e
        this.progress.running = false
        console.log(e)
        return
      }

      this.progress.running = false
    },

    sync: async function() {
      this.progress.running = true
      for (const syncFile of this.syncFileList) {
        syncFile.done = false
        syncFile.progress = true
        this.syncFileList.__ob__.dep.notify()

        try {
          switch(syncFile.action.action) {
            case "copy":
              await this.syncFile(this.api1, this.api2, syncFile.name)
              break

            case "delete":
              await fileAccessService.deleteFile(this.api2, syncFile.name)
              break
          }

        } catch(err) {
            this.error = err
            this.progress.running = false
        } finally {
          this.progress.running = false
          syncFile.done = true
          syncFile.progress = false
          this.syncFileList.__ob__.dep.notify()
        }        
      }            
    },

    syncFile: async function(api1, api2, path) {

      this.progress.running = true

      let oldTotalBytes = this.progress.overall.currentBytes

      const sourceData = await axios.get(api1 + "/file" + path, {
        responseType: 'blob',
        onDownloadProgress: (e) => {
          if (e.type === "progress") {    
              this.progress.currentFile.name = path
              this.progress.currentFile.currentBytes = e.loaded
              this.progress.currentFile.totalBytes = e.total * 2
              this.progress.overall.currentBytes = oldTotalBytes + this.progress.currentFile.currentBytes
          }
        }}).then((res) => {
          if (res.status !== 200) {
              const error = new Error(res.statusText)
              throw error
          }            
          return res.data
      })

      const formData = new FormData()
      formData.append('file', sourceData, path.split('/').pop())        

      await axios.post(api2 + "/file" + path.substring(0, path.lastIndexOf("/")), formData, {
      onUploadProgress: (e) => {
          if (e.type === "progress") {            
              this.progress.currentFile.name = path
              this.progress.currentFile.currentBytes = e.total + e.loaded
              this.progress.currentFile.totalBytes = e.total * 2
              this.progress.overall.currentBytes = oldTotalBytes + this.progress.currentFile.currentBytes
          }
      }}).then((res) => {
        if (res.status !== 201) {
          const error = new Error(res.statusText)
          throw error
        }            
      })  
    }    
  },

  data() {
    return {
      loading: false,
      error: false,
      syncFileList: [],
      listCreated: false,
      
      progress: {
        overall: {
          totalBytes: 0,
          currentBytes: 0
        },
        currentFile: {
          totalBytes: 0,
          currentBytes: 0,
          name: ""
        },
        count: {
          totalFiles: 0,
          currentFiles: 0
        },
        canceled: false,
        running: false
      },     
    }
  },

  props: ["api1", "api2"],
  name: "Sync",
}
</script>

<style scoped>
.sync {
}
</style>
