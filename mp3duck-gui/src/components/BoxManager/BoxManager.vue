<template>
  <b-card class="boxManager" header="">
    <ApiSelector @apiChange="changeApi" />
    <Cardview v-bind:api="api" />
    <Filebrowser v-bind:api="api" />
    <Backup v-bind:api="api" />    
  </b-card>
</template>

<script>

import Cardview from './Cardview.vue'
import Filebrowser from './Filebrowser.vue'
import Backup from './Backup.vue'
import ApiSelector from './ApiSelector.vue'

export default {
  components: {
    Cardview,
    Filebrowser,
    Backup,
    ApiSelector
  },
  methods: {
    changeApi(api) {
      this.api = api
      this.$emit('apiChange', this.api)
    }
  },
  data() {
    return {
      api: ""
    }
  },
  name: "BoxManager",
}
</script>

<style scoped>
.boxManager {
}
</style>
