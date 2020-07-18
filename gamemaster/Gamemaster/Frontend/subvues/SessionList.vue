<template>
    <div>
        SessionList
        <table>
            <thead>
                <tr>
                    <th>Timestamp</th>
                    <th>Name</th>
                    <th>Owner</th>
                </tr>
            </thead>
            <tbody>
                <SessionListElement v-for="d in tabledata" :data="d"></SessionListElement>
            </tbody>
        </table>
    </div>
</template>
<script lang="ts">
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import { defineComponent, ref } from "vue";
    import SessionListElement from "./SessionListElement.vue";
    export default defineComponent({
        components: {
            SessionListElement
        },
        data() {
            return { tabledata: [] };
        },
        mounted() {
            console.log("Setup");
            console.log(this);
            const options: AxiosRequestConfig = {
                method: 'GET',
                params: {"take":100,"skip":0},
                headers: { 'Content-Type': 'x-www-form-urlencoded' },
                url: '/api/gamesession/Listrecent',
            };
            axios(options).then(
                response => {
                    console.log(response);
                    console.log(this);
                    this.tabledata = response.data;
                });
            return;
        }
    });
</script>