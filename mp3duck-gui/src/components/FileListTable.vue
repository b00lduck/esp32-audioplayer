<template>
    <b-table :items="items" head-variant="light" sticky-header />
</template>

<script>

export default {

  computed: {
    items: function () {
        const items = []
        for (const syncFile of this.fileList) {               
          items.push({
            filename: syncFile.name,
            size: (syncFile.size / (1024*1024)).toFixed(1) + " MiB",
            done: syncFile.done === undefined ? false : syncFile.done,
            progress: syncFile.progress === undefined ? false : syncFile.progress,
            action: syncFile.action === undefined ? "copy" : syncFile.action.action
          })
        }
      return items
    },
  },
  props: ["fileList"],
  name: "FileListTable",
}
</script>

<style scoped>
div {
  font-size: 12px;
}
</style>
