<template>

  <div class="mappings">
    <ul>
      <li v-for="(mapping, index) in data" :key="index">
        {{ mapping }}
      </li>
    </ul>  
  </div>
</template>

<script>

  export default {
    methods: {
      fetchData: function() {
        this.loading = true

        return fetch("http://192.168.2.149/api/card", {
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
        data: [],
        loading: false,
        error: false
      }
    },
    created() {
      //this.fetchData()
    },
    name: "Mappings",
  }; 
</script>

<style scoped>
  .mappings {
    border: 1px solid black;
    text-align: left;
    width: 500px;    
  }
  li {
    padding-top: 4px;
  }
</style>
