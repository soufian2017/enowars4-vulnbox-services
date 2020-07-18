import { createRouter, createWebHistory } from 'vue-router'
import Home from './subvues/Home.vue'
import Login from './subvues/Login.vue'
import Register from './subvues/Register.vue'
import Sessions from './subvues/Sessions.vue'
import SessionList from './subvues/SessionList.vue'
import AccountSettings from './subvues/AccountSettings.vue'
import SessionDetails from './subvues/SessionDetails.vue'
import Tokens from './subvues/Tokens.vue'
import TokenDetails from './subvues/TokenDetails.vue'

const routerHistory = createWebHistory()

const router = createRouter({
    history: routerHistory,
    routes: [
        {
            path: '/',
            component: Home
        },
        {
            path: '/login',
            component: Login
        },
        {
            path: '/register',
            component: Register
        },
        {
            path: '/sessions',
            component: Sessions
        }, /*       
        {
            path: '/sessionList',
            component: SessionList
        }, */
        {
            path: '/accountSettings',
            component: AccountSettings
        },
        {
            path: '/sessionDetails/:id',
            name: 'SessionDetails',
            component: SessionDetails
        },
        {
            path: '/tokens',
            component: Tokens
        },
        {
            path: '/TokenDetails/:id/',
            name: 'TokenDetails',
            component: TokenDetails,
            props(route) {
                return route.params || {};
            }
        }
    ]
})

export default router