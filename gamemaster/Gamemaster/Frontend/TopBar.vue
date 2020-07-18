<template>
    <div class="topbar">
        <ul>
            <li>this</li>
            <li>is</li>
            <li>a</li>
            <li>component</li>
            <li><button @click="navigateToLogin">Login</button></li>
        </ul>
    </div>
</template>
<script lang="ts">
import { defineComponent, ref } from "vue";
export default defineComponent({
    setup() {
        const navigateToLogin = () => {
        }

        return {
            navigateToLogin,
        }
    }
});
</script>
