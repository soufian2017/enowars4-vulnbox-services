<template>
    <div id="sidebar" class="sidebar">
        <div id="chatcontainer" class="chatcontainer">
            <div id="chatbody">
                <h2>Chat History::</h2>
                <ChatMessageVue v-for="msg in chatdata" :data="msg"></ChatMessageVue>
            </div>
            <footer id="chatfooter" class="chatfooter">
                <form>
                    <input type="text" name="Message" v-model="input.msg" placeholder="Description" />
                    <button type="button" v-on:click="send()">Add</button>
                </form>
            </footer>
        </div>
    </div>
</template>
<script lang="ts">
    import { defineComponent, ref, reactive, readonly } from 'vue';
    import { gmState } from "../store/gmstate";
    import router from '../router';
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import ChatMessageVue from "./ChatMessageVue.vue";
    import { SignalRContext } from "./../scripts/signalrhelper";
    import { ChatMessage } from '../scripts/types';
    import * as moment from "moment";
    export default defineComponent({
        props: ['sessionId'],
        components: {
            ChatMessageVue
        },
        data() {
            return {
                chatdata:  [],
                input: {
                    msg: ""
                }
            };
        },
        mounted() {
            console.log("SidebarVue Mounted Called");
            console.log(this.$props.sessionId);
            var bodyFormData = new FormData();
            bodyFormData.set('id', this.$props.sessionId);
            const options: AxiosRequestConfig = {
                method: 'POST',
                data: bodyFormData,
                headers: { 'Content-Type': 'x-www-form-urlencoded' },
                url: '/api/chat/Getrecent',
            };
            axios(options).then(
                response => {
                    console.log("ChatMessages");
                    //this.chatdata = response.data;
                });
            var ctx: SignalRContext = SignalRContext.getInstance();
            ctx.setChatMessageHandler((s: ChatMessage[]) => {
                console.log("Chat Messages received: ", s)
                console.log("Chat Messages stored: ", this.chatdata)
                console.log("Session Context: ", this.$props.sessionId)
                var newdata: ChatMessage[] = s.concat(this.chatdata as ChatMessage[]).sort(function (a, b) { return a.id - b.id }).filter((e, i, a) => (i == 0) || e.id !== a[i - 1].id).filter((e, i, a) => e.sessionContextId == this.$props.sessionId)
                newdata.forEach(function (e, i, a) { e.timeString = moment(e.timestamp).format("HH:MM"); e.tooltip = moment(e.timestamp).format("DD/MM/YY, HH:MM:SS");});
                console.log("Chat Messages processed: ", newdata)
                this.chatdata = newdata as any;
            });
            return;
        },
        methods: {
            send() {
                var ctx: SignalRContext = SignalRContext.getInstance();
                ctx.sendmsg(this.input.msg);
            }
        }

    });

</script>

<style scoped>
    .sidebar {
        height: 100%;
        min-height: 100vh;
        width: 19%;
        float: left;
        z-index: 1;
        overflow-x: hidden;
        
        transition: 0.5s;
    }
    .chatcontainer {
        background-color: var(--bg-green);
        border: 1px solid blue;
        text-align: initial;
        height: 100%;
        min-height: 100vh;
        padding: 5px;
    }
    .chatfooter {
        left: 0;
        bottom: 0;
    }
    @media screen and (max-height: 450px) {
        .sidebar {
            padding-top: 5px;
        }
        .sidebar a {
            font-size: 18px;
        }
    }

</style>