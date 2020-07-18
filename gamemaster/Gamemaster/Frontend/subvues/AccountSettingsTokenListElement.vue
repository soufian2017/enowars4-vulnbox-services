<template>
    <router-link :to="{name: 'SessionDetails', params: {id:data.id} }">
        <tr>
            <td>{{data.id}}</td>
            <td>{{data.name}}</td>
            <td>{{data.ownerName}}</td>
        </tr>
    </router-link>
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