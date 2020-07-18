<template>
    <div>
        <h1>Token Details for Token {{input.tname}}</h1>
        <table>
            <tr>
                <td>
                    {{input.tname}}
                </td>
            </tr>
            <tr>
                <td>
                    {{input.description}}
                </td>
            </tr>
            <tr>
                <td>
                    {{input.ownername}}
                </td>
            </tr>
            <tr>
                <td>
                    <img ref="tokenimg" src="" />
                </td>
            </tr>
        </table>
    </div>
</template>

<script lang="ts">
    import { defineComponent } from 'vue';
    import { gmState } from './../store/gmstate';
    import router from './../router';
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    export default defineComponent({
        props: ['id', 'name'],
        data() {
            return {
                input: {
                    tname: "tokenname",
                    description: "",
                    isPrivate: true,
                    ownername: "",
                    id: ""
                }
            }
        },
        components: {
        },
        setup() {

        },
        mounted() {
            console.log(this.$props.id);
            const th = this as any;
            th.$refs.tokenimg.src = '/api/token/geticon?UUID=' + this.$props.id;
            var bodyFormData = new FormData();
            const foo = this as any;
            bodyFormData.set('UUID', this.$props.id);
            this.input.id = foo.$route.params.id;
            const options: AxiosRequestConfig = {
                method: 'POST',
                data: bodyFormData,
                headers: { 'Content-Type': 'x-www-form-urlencoded' },
                url: '/api/token/info'
            };
            axios(options).then(
                response => {
                    if (response.status == 200) {
                        console.log("Response from info:");
                        console.log(response.data);
                        this.input.tname = response.data["name"];
                        this.input.description = response.data["description"];
                        this.input.isPrivate = response.data["isPrivate"];
                        this.input.ownername = response.data["ownerName"];
                        console.log(this.input);
                    } else {
                        console.log("this should not happen...");
                    }
                }).catch(error => {
                    console.log(error);
                }); 
            return {
            }
        },
        methods: {
        }
    });
</script>