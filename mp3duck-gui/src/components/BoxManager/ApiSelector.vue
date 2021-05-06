<template>
  
  <b-card class="apiSelector" header="API">    
    <b-button-group>      
      <b-form-input v-model="api"/>
      <b-button style="width: 130px;" @click="changeApi">Set API</b-button>
    </b-button-group>
  </b-card>
  
</template>

<script>

  export default {
    components: { },
    methods: { 
      changeApi() {
        this.$emit('apiChange', this.api)
      }
    },
    beforeMount() {
      this.$emit('apiChange', this.api)
    },
    data() {
      return {
        api: "http://192.168.2.149/api"
      }
    },
    name: "ApiSelector"
  }; 
</script>

<style scoped>
.apiSelector {
}
</style>
