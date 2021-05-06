<template>
  <b-card class="backup" header="Backup and Restore">
    <div v-if="progress.running">
      <CopyProgress v-bind:progress="progress" />
      <br />
      <FileListTable v-bind:fileList="backupFileList" />
    </div>

    <b-button-toolbar>
      <b-button-group class="mx-2">
        <div v-if="!progress.running">
          <b-button variant="primary" v-on:click="doBackup()">create backup archive</b-button>
        </div>
        <div v-if="progress.running">
          <b-button variant="danger" v-on:click="cancel()">abort backup process</b-button>
        </div>
      </b-button-group>
    </b-button-toolbar>
  </b-card>
</template>

<script>
import JSZip from "jszip"
import { saveAs } from "file-saver"
import CopyProgress from "../CopyProgress"
import FileListTable from "../FileListTable"
import FileAccessService from "../../service/FileAccessService"

const fileAccessService = new FileAccessService()

export default {
  components: {
    CopyProgress,
    FileListTable
  },
  computed: {
    currentBytes: function () {
      return this.doneBackupBytes + this.currentFile.currentBytes
    },
  },
  methods: {

    cancel: async function () {
      this.progress.canceled = true
    },

    doBackup: async function () {

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
        let { fileList, totalBytes, totalFiles } = await fileAccessService.getBackupList(this.api)
        this.progress.overall.totalBytes = totalBytes
        this.progress.count.totalFiles = totalFiles
        this.backupFileList = fileList

        const zip = new JSZip();

        for (const backupFile of this.backupFileList) {    
          if (this.progress.canceled) {
            this.progress.running = false
            return
          }
          backupFile.progress = true
          this.backupFileList.__ob__.dep.notify()
          this.progress.currentFile.name = backupFile.name
          const data = await fileAccessService.fetchFileWithProgress(this.api, backupFile.name, this.progress)
          zip.file(backupFile.name, data, { binary: true, createFolders: true })
          this.progress.count.currentFiles++
          backupFile.done = true
          backupFile.progress = false
          this.backupFileList.__ob__.dep.notify()
        }

        zip
          .generateAsync({ type: "blob" }, (f) => {
            console.log(f)
          })
          .then((content) => {
            saveAs(content, "example-backup.zip")
          })

        this.progress.running = false;
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
      backupFileList: [],
      
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

  props: ["api"],
  name: "Backup",
}
</script>

<style scoped>
.backup {
}
table {
  font-size: 8px;
}
</style>
