<template>
    <div class="container">
        <div id='nav'>
            <h1>Gamemaster Service</h1>
            <router-link to='/'>Home</router-link>
            <router-link to='/login'>Login</router-link>
            <router-link to='/register'>Register</router-link>
            <router-link to='/sessions'>Sessions</router-link>
            <router-link v-if="state.username != null" to='/tokens'>Tokens</router-link>
            <router-link v-if="state.username != null" to='/'><span v-on:click="logout()">{{state.username}}(Logout)</span></router-link>
        </div>
        <router-view />
    </div>
</template>
<script lang="ts">
    import { defineComponent, ref, reactive, readonly } from 'vue';
    import { gmState } from "./store/gmstate";
    import router from './router';
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import { SignalRContext } from "./scripts/signalrhelper";
    export default defineComponent({
        setup() {
            const options: AxiosRequestConfig = {
                method: 'POST',
                headers: { 'Content-Type': 'x-www-form-urlencoded' },
                url: '/api/account/info',
            }
            axios(options).then(
                response => {
                    console.log(response);
                    if (response.status == 200) {
                        var username = response.data["name"];
                        console.log("Login Successful as " + username);
                        SignalRContext.getInstance().ensureConnected();
                        console.log(response);
                        gmState.login(username);
                    } else {
                        console.log("this should not happen...");
                    }
                }).catch(error => {
                    console.log("No Login found");
                    console.log(error);
                })
            return {
                state: gmState.getState()
            }
        },
        mounted() {
            let style = document.createElement('link');
            style.type = "text/css";
            style.rel = "stylesheet";
            style.href = '';
            document.head.appendChild(style);
        },
        methods: {
            logout() {
                console.log("Logging out...");
                const options: AxiosRequestConfig = {
                    method: 'POST',
                    headers: { 'Content-Type': 'x-www-form-urlencoded' },
                    url: '/api/account/logout',
                };
                axios(options).then(
                    response => {
                        console.log(response);
                        if (response.status == 200) {
                            console.log("Logoff Successful");
                            alert("Logoff Successful");
                            gmState.logoff();
                            router.push("/");
                        } else {
                            console.log("this should not happen...");
                        }
                    }).catch(error => {
                        console.log("Logoff failed");
                        console.log(error);
                        alert("Logoff failed");
                    })
                return false;
            }
        }
    });
</script>
<style>
    @import './assets/style.css';
</style>
<style scoped>
    .container {
        padding: 20px 20px 20px 20px;
    }
    :root {

    }
    #root {

        text-align: center;
        font-family: 'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;
    }

    img {
        width: 200px;
    }
    h1 {
        display: inline;
        color: var(--fg-orange);
    }
    #nav {
        font-size: 1.5em;
        margin-bottom: 30px;
    }

    a {
        text-decoration: none;
        margin: 30px 25px;
        color: var(--fg-yellow);
    }

        a:hover {
            text-decoration: underline;
            color:var(--fg-red);
        }
</style>
