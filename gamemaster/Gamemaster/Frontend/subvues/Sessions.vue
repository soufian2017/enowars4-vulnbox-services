<template>
    <div id="sessions">
        Session Manager
        <SessionAdd></SessionAdd>
        <SessionList></SessionList>
    </div>
</template>

<script lang="ts">
    import { defineComponent } from 'vue';
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import router from './../router';
    import { METHODS } from 'http';
    import { gmState } from "../store/gmstate";
    import SessionAdd from "./SessionAdd.vue";
    import SessionList from "./SessionList.vue";
    export default defineComponent({
        components: {
            SessionAdd,
            SessionList
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