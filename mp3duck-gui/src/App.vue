<template>
  <div id="app">
    <h1>MP3 Duck</h1>
    <Cardview />
    <Mappings />
    <Filebrowser />
  </div>
</template>

<script>
import Cardview from './components/Cardview.vue'
import Mappings from './components/Mappings.vue'
import Filebrowser from './components/Filebrowser.vue'

export default {
  name: 'App',
  components: {
    Cardview,
    Mappings,
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
