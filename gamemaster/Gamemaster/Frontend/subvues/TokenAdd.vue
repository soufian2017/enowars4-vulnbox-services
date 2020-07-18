<template>
    <div id="tokenAdd">
        <h1>Add Token</h1>
        <form id="addtoken" ref="addtoken"
              action=""
              method="post">
            <input type="text" name="name" placeholder="Name" />
            <input type="text" name="description" placeholder="Description" />
            <input type="checkbox" name="isPrivate" placeholder="Description" />
            <input type="file" name="icon" />
            <button type="button" v-on:click="add()">Add</button>
        </form>
    </div>
</template>

<script lang="ts">
    import { defineComponent } from 'vue';
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import router from './../router';
    import { METHODS } from 'http';
    import { gmState } from "../store/gmstate";
    export default defineComponent({
        data() {
            return {
            }
        },
        methods: {
            add() {
                var bodyFormData = new FormData(this.$refs.addtoken as HTMLFormElement);
                if (bodyFormData.get("isPrivate") == "on")
                {
                    bodyFormData.set("isPrivate", "true")
                } else
                {
                    bodyFormData.set("isPrivate", "false");
                }
                console.log(bodyFormData);
                console.log(bodyFormData.get("isPrivate"));
                console.log("Adding Token...");
                const options: AxiosRequestConfig = {
                    method: 'POST',
                    data: bodyFormData,
                    headers: { 'Content-Type': 'multipart/form-data' },
                    url: '/api/account/AddToken',
                };
                axios(options).then(
                    response => {
                        console.log(response);
                        if (response.status == 200) {
                            console.log("Token Added Successfully");
                            alert("Token Added Successfully");
                            router.push("/tokens");
                        } else {
                            console.log("this should not happen...");
                        }
                    }).catch(error => {
                        console.log("Token Add failed");
                        console.log(error);
                        alert("Token Add failed");
                    })
                return false;
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