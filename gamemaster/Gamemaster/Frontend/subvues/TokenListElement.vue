<template>
    <tr class="tr-h" v-on:click="routerlinkto">
        <td>{{data.name}}</td>
        <td>{{data.description}}</td>
    </tr>
</template>

<script lang="ts">
    import { defineComponent, ref } from "vue";
    import router from './../router';
    export default defineComponent({
        props: ['data'],
        setup(props) {
            console.log(props['data']);
            return;
        },
        methods: {
            routerlinkto() {
                router.push({ name: 'TokenDetails', params: { id: this.$props['data'].uuid ,  name: this.$props['data'].name } });
            }
        }
    });
</script>

<style scoped>
    tr.tr-h:hover {
        cursor: pointer;
    }
</style>