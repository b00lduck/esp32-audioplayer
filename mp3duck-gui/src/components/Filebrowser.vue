<template>

  <div class="filebrowser">
    <h1>Filebrowser</h1>
    <h2>Current path: {{ path }}</h2>

    <button v-on:click="goParent()">parent dir</button>
    <button v-on:click="goRoot()">root dir</button>
    
    <h4 v-if="loading">loading...</h4>

    <b-list-group v-if="!loading">
      <b-list-group-item 
          v-on:click="goDir(file)" 
          class="d-flex justify-content-between align-items-center" 
          v-for="(file, index) in data" 
          :key="index"
          v-bind:class="file.type">       

        <b-icon v-bind:icon="icon(file.type)" scale="2" />

        {{ file.name }}
      </b-list-group-item>
    </b-list-group>

  </div>
</template>

<script>
  export default {
    methods: {
      icon: function(icon) {
        return {
          "directory": "folder",
          "file": "file-earmark"
        }[icon]
      },

      goParent: function() {        
        if (this.path != "/") {
          this.oldPath = this.path
          const parts = this.path.split("/")
          this.path = "/"
          for(let i=0;i<parts.length-1;i++) {
            this.path = this.path + parts[i]
          }
          this.fetchData();          
        }
      },

      goRoot: function() {        
        this.goDir({name:"/", type: "directory"})
      },

      goDir: function(dir) {
        if ((this.path != dir.name) && (dir.type == "directory")) {
          this.oldPath = this.path
          this.path = dir.name;
          this.fetchData();          
        }          
      },

      fetchData: function() {
        this.loading = true

        return fetch("http://192.168.2.149/api/file" + this.path, {
          method: "get"
        })
          .then((res) => {
            if (!res.ok) {
              const error = new Error(res.statusText);
              throw error;
            }
            return res.json()
          })
          .then((data) => {
            this.data = data
          })
          .catch((err) => {
            this.error = true
            this.path = this.oldPath
            console.log(err)
          })
          .then(() => {
            this.loading = false;
          });
      }
    },

    data() {
      return {
        data: [],
        loading: false,
        error: false,
        path: "/",
        oldPath: "/"
      }
    },

    created() {
      this.fetchData()
    },

    name: "Filebrowser",
  }; 
</script>

<style scoped>
  .filebrowser {
    border: 1px solid black;
    text-align: left;
    width: 500px; 
  }

  .directory {
    cursor: pointer;    
  }

  .directory:hover {
    background-color: #bbb;
  }
</style>
