<template>
  <b-card class="boxManager" header="">
    <ApiSelector @apiChange="changeApi" />
    <Cardview v-bind:api="api" />
    <Filebrowser v-bind:api="api" />
    <Backup v-bind:api="api" />    
    <CardList v-bind:api="api" />
  </b-card>
</template>

<script>

import Cardview from './Cardview.vue'
import Filebrowser from './Filebrowser.vue'
import Backup from './Backup.vue'
import ApiSelector from './ApiSelector.vue'
import CardList from './CardList.vue'

export default {
  components: {
    Cardview,
    Filebrowser,
    Backup,
    ApiSelector,
    CardList
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
  background-color: brown;
}
</style>
