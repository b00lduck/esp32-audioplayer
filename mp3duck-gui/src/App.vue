<template>
  <div id="app">
    <h1>MP3 Duck</h1>
    <Cardview />
    <Filebrowser />
  </div>
</template>

<script>
import Cardview from './components/Cardview.vue'
import Filebrowser from './components/Filebrowser.vue'
import '@progress/kendo-ui' 
import '@progress/kendo-theme-default/dist/all.css'

export default {
  name: 'App',
  components: {
    Cardview,
    Filebrowser
  }
}
</script>

<style>
  #app {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    margin-top: 60px;
  }
</style>
