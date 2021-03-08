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
  export default {
    props: ['entry'],
    methods: {
      deleteEntry: function() {
        return fetch("http://192.168.2.149/api/file" + this.entry.path, {
          method: "delete"
        })
        .then((res) => {
          if (!res.ok) {
            const error = new Error(res.statusText);
            throw error;
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
