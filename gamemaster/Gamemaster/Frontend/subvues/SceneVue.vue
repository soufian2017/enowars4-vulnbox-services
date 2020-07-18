<template>
    <div id="scene" class="scene rightexpanded">
    </div>
</template>
<script lang="ts">
    import { defineComponent, ref, reactive, readonly } from 'vue';
    import { gmState } from "../store/gmstate";
    import router from '../router';
    import axios, { AxiosRequestConfig, AxiosPromise, AxiosResponse } from 'axios';
    import { CombatScene } from "./../scripts/phasergame";
    import { SignalRContext } from "./../scripts/signalrhelper";
    import { Scene } from "./../scripts/types";

    export default defineComponent({
        props: ['sessionId'],
        data() {
            return {
                
            }
        },
        components: {
        },
        mounted() {
            const gameConfig: Phaser.Types.Core.GameConfig = {
                title: 'Sample',
                type: Phaser.AUTO,
                scale: {
                    width: 800,
                    height: 600,
                },
                physics: {
                    default: 'arcade',
                    arcade: {
                        debug: true,
                    },
                },
                parent: 'scene',
                backgroundColor: '#000000',
                scene: CombatScene
            };
            var ctx: SignalRContext = SignalRContext.getInstance();
            const game = new Phaser.Game(gameConfig);
            var sid = Number(this.sessionId);

            ctx.setSessionId(sid);
        },
        beforeUnmount() {
            var ctx: SignalRContext = SignalRContext.getInstance();
        }
    });
</script>

<style scoped>
    .scene {
        display:inline-block;
        height: 100%;
        width: 60%;
        z-index: 1;
        overflow-x: hidden;
        transition: 0.5s;
    }
    .rightexpanded {
    }

    @media screen and (max-height: 450px) {
        .sidebar {
            padding-top: 5px;
        }

        .sidenav a {
            font-size: 18px;
        }
    }
</style>