<template>
    <div id="login">
        <h1>Login</h1>
        <form id="login"
              action=""
              method="post">
            <input type="text" name="username" v-model="input.username" placeholder="Username" />
            <input type="password" name="password" v-model="input.password" placeholder="Password" />
            <button type="button" v-on:click="login()">Login</button>
        </form>
    </div>
</template>

<script lang="ts">
    import { defineComponent } from 'vue';
    import axios, { AxiosRequestConfig } from 'axios';
    import router from './../router';
    import { gmState } from "../store/gmstate";
    import { SignalRContext } from "./../scripts/signalrhelper";
    export default defineComponent({
        name: 'Login',
        data() {
            return {
                input: {
                    username: "",
                    password: ""
                }
            }
        },
        methods: {
            login() {
                var bodyFormData = new FormData();
                bodyFormData.set('userName', this.input.username);
                bodyFormData.set('password', this.input.password);
                console.log("Logging in...");
                const options: AxiosRequestConfig = {
                    method: 'POST',
                    data: bodyFormData,
                    headers: { 'Content-Type': 'x-www-form-urlencoded' },
                    url: '/api/account/login',
                };
                axios(options).then(
                    response => {
                        console.log(response);
                        if (response.status == 200) {
                            console.log("Login Successful");
                            alert("Login Successful");
                            gmState.login(this.input.username);
                            SignalRContext.getInstance().ensureConnected();
                            router.push("/");
                        } else {
                            console.log("this should not happen...");
                        }
                    }).catch(error => {
                        console.log("Login failed");
                        console.log(error);
                        alert("Login failed");
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