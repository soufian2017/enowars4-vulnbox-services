<template>
    <div id="rightbar" class="rightbar">
        Tokens to Add:
    </div>
</template>
<script lang="ts">
    import { defineComponent, ref, reactive, readonly } from 'vue';
    import { gmState } from "../store/gmstate";
    import router from '../router';
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    export default defineComponent({
        props: ['sessionId'],
        components: {
        },
        data() {
            return { tokendata: [] };
        },
        mounted() {

        }

    });

</script>

<style scoped>
    .rightbar {
        float: right;
        height: 100%;
        min-height: 100vh;
        width: 20%;
        z-index: -1;
        overflow-x: hidden;
        padding: 60px 0px 60px 0px;
        transition: 0.5s;
    }
    @media screen and (max-height: 450px) {
        .rightbar {
            padding-top: 5px;
        }

            .rightbar a {
                font-size: 18px;
            }
    }
</style>