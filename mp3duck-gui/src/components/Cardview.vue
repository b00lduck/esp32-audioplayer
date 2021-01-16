<template>
  <div class="card">
    <br />
    <h1>Card ID:</h1>
    <h2 v-if="!error">{{ id }}</h2>    
    <h2 v-if="error">ERROR</h2><br>    
  </div>
</template>

<script>

  export default {
    methods: {
      fetchData: function() {
        this.loading = true

        return fetch("http://192.168.2.149/api/card/current", {
          method: "get"
        })
          .then((res) => {
            if (!res.ok) {
              const error = new Error(res.statusText);
              throw error;
            }
            return res.text()
          })
          .then((data) => {
            this.id = data
          })
          .catch((err) => {
            this.error = true
            console.log(err)
            setTimeout(this.fetchData, 1000);
          })
          .then(() => {
            this.loading = false;
            setTimeout(this.fetchData, 1000);
          });
      }
    },
    data() {
      return {
        id: "no card",
        loading: false,
        error: false
      }
    },
    created() {
      //this.fetchData()
    },
    name: "Cardview",
  }; 
</script>

<style scoped>
.card {
  border: 1px solid black;
  border-radius: 10px;
  width: 300px;
  height: 170px;
  margin: auto;
}
</style>
