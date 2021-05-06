<template>
  <div id="app">
    <h1>MP3 Duck</h1>
    <b-container>
      <b-row>
        <b-col><BoxManager @apiChange="changeApi1" /></b-col>
        <b-col cols="1">vs.</b-col>
        <b-col><BoxManager @apiChange="changeApi2" /></b-col>
      </b-row>
      <b-row>
        <b-col cols="3"></b-col>
        <b-col><Sync v-bind:api1="api1" v-bind:api2="api2" /></b-col>
        <b-col cols="3"></b-col>
      </b-row>
    </b-container>
  </div>

</template>

<script>

import BoxManager from './components/BoxManager/BoxManager'
import Sync from './components/Sync'
import '@progress/kendo-ui' 
import '@progress/kendo-theme-default/dist/all.css'

export default {
  name: 'App',
    methods: {
    changeApi1(api) {
      this.api1 = api
    },
    changeApi2(api) {
      this.api2 = api
    }
  },
  data() {
    return {
      api1: "",
      api2: ""
    }
  },  
  components: {
    BoxManager,
    Sync
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
