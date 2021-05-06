<template>
  <div class="entry d-flex justify-content-between align-items-center text-align-left">
    <b-button size="sm" disabled style="width: 45px">{{entry.id}}</b-button>
    <b-form-input v-b-tooltip v-bind:title="entry.path" size="sm" v-bind:value="entry.name" disabled />
    <b-button variant="danger" v-on:click="deleteEntry" size="sm">
      <b-icon size="sm" icon="x-circle"/>
    </b-button>                      
  </div>
</template>

<script>

  import axios from 'axios'

  export default {
    props: ['entry', 'api'],
    methods: {
      deleteEntry: function() {       

        const segments = this.entry.path.substr(1).split("/")
        let url = this.api + "/file"
        for (const segment of segments) {
          url += "/" +  encodeURIComponent(segment)
        }        

        return axios.delete(url)
        .then((res) => {
          if (res.status !== 204) {
            this.$emit('error', new Error("deleting playlist entry failed: HTTP " + res.statusText))
          }          
        })
      }
    },
    data() {
      return {}
    },
    name: "PlaylistEntry",
  }; 
</script>

<style scoped>
  .entry {
    width: 100%;
    padding-top: 2px;
  }
</style>
