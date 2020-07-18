import {Store} from "./main";
import { watch, ref, reactive } from 'vue';

interface Data {
    username: null|string
}

class GmState extends Store<Data> {
    protected data(): Data {
        return {
            username: null
        };
    }
    logoff() {
        this.state.username = null;
    }
    login(username: string) {
        this.state.username = username;
    }
}

export const gmState: GmState = new GmState()