<template>
    <tr class="tr-h" v-on:click="routerlinkto">
        <td>{{data.id}}</td>
        <td>{{data.name}}</td>
        <td>{{data.ownerName}}</td>
    </tr>
</template>

<script lang="ts">
    import { defineComponent, ref } from "vue";
    import router from './../router';
    export default defineComponent({
        props: ['data'],
        setup(props) {
            console.log("Setup SList prop:");
            console.log(props['data'].id);
            return;
        },
        methods: {
            routerlinkto() {
                router.push({ name: 'SessionDetails', params: { id: this.$props['data'].id } });
            }
        }
    });
</script>

<style scoped>
    tr.tr-h:hover {
        cursor: pointer;
    }
</style>