<template>
    <tr>
        <td title="{{data.tooltip}}">({{data.timeString}})</td>
        <td>{{data.senderName}}</td>
    </tr>
    <tr>
        <td>{{data.content}}</td>
    </tr>
</template>

<script lang="ts">
    import { defineComponent, ref } from "vue";
    export default defineComponent({
        props: ['data'],
        setup(props) {
            console.log(props['data'].id);
            return;
        }
    });
</script>

<style scoped>
</style>