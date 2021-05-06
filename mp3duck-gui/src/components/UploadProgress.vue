<template>

    <div class="progress">
      Uploading: {{ currentMiBytesCurrentFile }}/{{ totalMiBytesCurrentFile }} MiB<br>
      <b-progress class="mt-2" :max="totalMiBytesCurrentFile" show-value>
        <b-progress-bar :value="currentMiBytesCurrentFile" variant="success" />
      </b-progress>

    </div>

</template>

<script>

export default {
  computed: {
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
}
</style>
