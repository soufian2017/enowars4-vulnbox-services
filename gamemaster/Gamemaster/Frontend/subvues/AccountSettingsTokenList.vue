<template>
    <div>
        TokenList
        <table>
            <thead>
                <tr>
                    <th>Name</th>
                    <th>Description</th>
                    <th>IsPrivate</th>
                </tr>
            </thead>
            <tbody>
                <AccountSettingsTokenListElement v-for="d in tabledata" :data="d"></AccountSettingsTokenListElement>
            </tbody>
        </table>
    </div>
</template>
<script lang="ts">
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import { defineComponent, ref } from "vue";
    import AccountSettingsTokenListElement from "./AccountSettingsTokenListElement.vue";
    export default defineComponent({
        components: {
            AccountSettingsTokenListElement
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
                url: '/api/gamesession/Listrecentsessions',
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