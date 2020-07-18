<template>
    <div id="login">
        <h1>Register</h1>
        <form id="register"
              action=""
              method="post">
            <input type="text" name="username" v-model="input.username" placeholder="Username" />
            <input type="text" name="email" v-model="input.email" placeholder="Email" />
            <input type="password" name="password" v-model="input.password" placeholder="Password" />
            <button type="button" v-on:click="register()">Register</button>
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
                    email: "",
                    password: ""
                }
            }
        },
        methods: {
            register() {
                var bodyFormData = new FormData();
                bodyFormData.set('userName', this.input.username);
                bodyFormData.set('email', this.input.email);
                bodyFormData.set('password', this.input.password);
                console.log("Register...");
                var options: AxiosRequestConfig = {
                    method: 'POST',
                    data: bodyFormData,
                    headers: { 'Content-Type': 'x-www-form-urlencoded' },
                    url: '/api/account/register',
                };
                axios(options).then(
                    response => {
                        console.log(response);
                        if (response.status == 200) {
                            console.log("Register Successful");
                            alert("Register Successful");
                            gmState.login(this.input.username);
                            SignalRContext.getInstance().ensureConnected();
                            router.push("/");
                        } else {
                            console.log("this should not happen...");
                        }
                    }).catch(error => {
                        console.log("Register failed");
                        console.log(error);
                        alert("Register failed");
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