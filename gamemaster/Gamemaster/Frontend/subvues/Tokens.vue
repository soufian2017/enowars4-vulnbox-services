<template>
    <div id="tokens">
        Token Manager
        <TokenAdd></TokenAdd>
        <TokenList></TokenList>
    </div>
</template>

<script lang="ts">
    import { defineComponent } from 'vue';
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import router from './../router';
    import { METHODS } from 'http';
    import { gmState } from "../store/gmstate";
    import TokenAdd from "./TokenAdd.vue";
    import TokenList from "./TokenList.vue";
    export default defineComponent({
        components: {
            TokenAdd,
            TokenList
        },
        data() {
            return {
                input: {
                }
            }
        }
    })
</script>

<style scoped>
    #login {
        width: 500px;
        border: 1px solid #CCCCCC;
        background-color: #FFFFFF;
        margin: auto;
        margin-top: 200px;
        padding: 20px;
    }
</style>