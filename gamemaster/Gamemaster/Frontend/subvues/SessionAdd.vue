<template>
    <div id="sessionAdd">
        <h1>Add Session</h1>
        <form id="addsession"
              action=""
              method="post">
            <input type="text" name="name" v-model="input.name" placeholder="Name" />
            <input type="text" name="notes" v-model="input.notes" placeholder="Notes" />
            <input type="password" name="password" v-model="input.password" placeholder="Password" />
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
                input: {
                    name: "",
                    notes: "",
                    password: ""
                }
            }
        },
        methods: {
            add() {
                var bodyFormData = new FormData();
                bodyFormData.set('name', this.input.name);
                bodyFormData.set('notes', this.input.notes);
                bodyFormData.set('password', String(this.input.password));
                console.log("Adding Session...");
                const options: AxiosRequestConfig = {
                    method: 'POST',
                    data: bodyFormData,
                    headers: { 'Content-Type': 'x-www-form-urlencoded' },
                    url: '/api/gamesession/create',
                };
                axios(options).then(
                    response => {
                        console.log(response);
                        if (response.status == 200) {
                            console.log("Session Added Successfully");
                            alert("Session Added Successfully");
                            router.push("/sessions");
                        } else {
                            console.log("this should not happen...");
                        }
                    }).catch(error => {
                        console.log("Session Add failed");
                        console.log(error);
                        alert("Session Add failed");
                    })
                return false;
            }
        }
    })
</script>

<style scoped>

</style>