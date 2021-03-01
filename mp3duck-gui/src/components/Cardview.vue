<template>
    <b-card class="card" header="Card info">    
      <div v-if="error">ERROR</div>
      <div v-if="!error">
        
        <b-row>
          <b-col sm="4">
            <b-label for="cardId">Card ID:</b-label>
          </b-col>
          <b-col sm="8">
            <b-form-input size="sm" id="cardId" v-model="card.id" disabled />
          </b-col>
        </b-row>

        <b-row>
          <b-col sm="4">
            <b-label for="cardName">Card name:</b-label>
          </b-col>
          <b-col sm="8">
            <b-form-input size="sm" id="cardName" v-model="card.name" disabled />
          </b-col>
        </b-row>

        <b-row>
          <b-col sm="4">
            <b-label for="cardStatus">Card status:</b-label>
          </b-col>
          <b-col sm="8">
            <b-form-input size="sm" id="cardStatus" v-model="card.status" disabled />
          </b-col>
        </b-row>        

      </div>
    </b-card>
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
            return res.json()
          })
          .then((data) => {
            if (data.id == "00000000") {
              data.id = "no card"
              data.name = "n/a"
              data.status = "n/a"
            }
            this.card = data
          })
          .catch((err) => {
            this.error = true
            console.log(err)
            setTimeout(this.fetchData, 5000);
          })
          .then(() => {
            this.loading = false;
            setTimeout(this.fetchData, 2000);
          });
      }
    },
    data() {
      return {
        card: {
          id: "",
          name: "",
          status: ""
        },
        loading: false,
        error: false
      }
    },
    created() {
      this.fetchData()
    },
    name: "Cardview",
  }; 
</script>

<style scoped>
.card {
  width: 600px;
}
</style>
