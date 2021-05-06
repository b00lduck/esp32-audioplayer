<template>

  <b-card class="filebrowser" header="Filebrowser">

    <div v-if="error">HTTP error<br>{{error}}</div>

    <b-button-toolbar>
      <b-button-group class="mx-2">
        <b-button  class="loading" variant="primary">
          <b-spinner v-if="loading" small/>
          <b-icon v-if="!loading" size="md" icon="arrow-clockwise"/>
        </b-button>
      </b-button-group>

      <b-input-group size="md">        
        <b-form-input disabled v-bind:value="path"></b-form-input>
      </b-input-group>
      <b-button-group class="mx-2">
        <b-button :disabled="isParent()" v-on:click="goParent()">parent dir</b-button>
        <b-button :disabled="isParent()" v-on:click="goRoot()">root dir</b-button>
      </b-button-group>      
    </b-button-toolbar>

    <br/>   

    <b-list-group v-if="!loading">
      <b-list-group-item 
          v-on:click="goDir(file)" 
          class="d-flex justify-content-between align-items-center text-align-left" 
          v-for="(file, index) in data" 
          :key="index"
          v-bind:class="file.type">       

        <b-icon v-bind:icon="icon(file.type)" scale="2" />

        <div>{{ file.name }}</div>
      </b-list-group-item>
    </b-list-group>

  </b-card>
</template>

<script>
  export default {
    methods: {
      icon: function(icon) {
        return {
          "directory": "folder",
          "file_audio": "file-earmark-music",
          "file_text": "file-earmark-text",
          "file_other": "file-earmark"
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

      isParent: function() {
        return (this.path == "/")
      },

      fetchData: function() {
        this.loading = true

        return fetch(this.api + "/file" + this.path, {
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
        })
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

    watch: {      
      api() {
        this.fetchData()
      }
    },    

    props: ['api'],
    name: "Filebrowser",
  }; 
</script>

<style scoped>
  .filebrowser {
  }

  .directory {
    cursor: pointer;    
  }

  .directory:hover {
    background-color: #bbb;
  }

  .loading {
    width: 45px;
  }
</style>
