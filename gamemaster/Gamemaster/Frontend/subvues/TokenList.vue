<template>
    <div>
        Tokens:
        <table>
            <thead>
                <tr>
                    <th>Timestamp</th>
                    <th>Name</th>
                    <th>Owner</th>
                </tr>
            </thead>
            <tbody>
                <TokenListElement v-for="d in tabledata" :data="d"></TokenListElement>
            </tbody>
        </table>
    </div>
</template>
<script lang="ts">
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import { defineComponent, ref } from "vue";
    import TokenListElement from "./TokenListElement.vue";
    export default defineComponent({
        components: {
            TokenListElement
        },
        data() {
            return { tabledata: [] };
        },
        mounted() {
            const options: AxiosRequestConfig = {
                method: 'POST',
                headers: { 'Content-Type': 'x-www-form-urlencoded' },
                url: '/api/token/list',
            };
            axios(options).then(
                response => {
                    console.log("TokenList");
                    console.log(this);
                    this.tabledata = response.data;
                });
            return;
        }
    });
</script>