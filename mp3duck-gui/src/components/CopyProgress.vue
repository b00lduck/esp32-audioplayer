<template>

    <div class="copyProgress">

      <h4>Upload in progress...</h4>

      <div v-if="progress.count">
        All files: {{ currentMiBytes }}/{{ totalMiBytes }} MiB ({{ this.rateKiBytesPerSec }} kiB/s)
        <b-progress class="mt-2" :max="totalMiBytes" show-value>
          <b-progress-bar striped animated :value="currentMiBytes" variant="success">
            {{ ((currentMiBytes/totalMiBytes)*100).toFixed(0) }} %
          </b-progress-bar>
        </b-progress>
      
        {{ progress.count.currentFiles }}/{{ progress.count.totalFiles }} Files
        <b-progress class="mt-2" :max="progress.count.totalFiles" show-value>
          <b-progress-bar striped animated :value="progress.count.currentFiles" variant="info" />
        </b-progress>
      </div>

      Current file: {{ currentMiBytesCurrentFile }}/{{ totalMiBytesCurrentFile }} MiB ({{ this.rateKiBytesPerSec }} kiB/s)<br>
      Filename: {{ progress.currentFile.name }}
      <b-progress class="mt-2" :max="totalMiBytesCurrentFile" show-value>
        <b-progress-bar striped animated :value="currentMiBytesCurrentFile" variant="primary">
          {{ ((currentMiBytesCurrentFile/totalMiBytesCurrentFile)*100).toFixed(0) }} %
        </b-progress-bar>
      </b-progress>

    </div>

</template>

<script>

export default {
  computed: {
    currentMiBytes: function() {
      return (this.progress.overall.currentBytes / (1024*1024)).toFixed(1)
    },
    totalMiBytes: function() {
      return (this.progress.overall.totalBytes / (1024*1024)).toFixed(1)
    },
    currentMiBytesCurrentFile: function() {
      return (this.progress.currentFile.currentBytes / (1024*1024)).toFixed(1)
    },
    totalMiBytesCurrentFile: function() {
      return (this.progress.currentFile.totalBytes / (1024*1024)).toFixed(1)
    },    
    rateKiBytesPerSec: function() {
      return (this.dataRate  / 1024).toFixed(1)
    },
  },

  methods: {
    calcDataRate: function() {
      const delta = this.progress.overall.currentBytes - this.dataRateLast      
      this.dataRate = delta / 2
      this.dataRateLast = this.progress.overall.currentBytes
    }
  },

  created() {
    this.interval = setInterval(this.calcDataRate, 1000)
  },

  beforeDestroy() {
    clearInterval(this.interval)
  },

  data() {
    return {      
      interval: undefined,
      dataRate: 0,
      dataRateLast: 0
    };
  },

  props: [
    "progress"
  ],
  name: "CopyProgress",
};
</script>

<style scoped>
.copyProgress {
  margin: 10px;
  padding: 10px;
  border-radius: 10px;
  border: 1px solid black;
}
</style>
