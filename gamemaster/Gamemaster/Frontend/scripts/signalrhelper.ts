import * as SignalR from "@microsoft/signalr";
import { Scene, ChatMessage } from "./types";

export class SignalRContext {
    private static instance: SignalRContext | null = null;
    public connection: SignalR.HubConnection;
    private handleSceneUpdate: ((s: Scene) => void) | null = null;
    private lastSceneUpdate: Scene | null = null;
    private sessionId: number | null = null;
    private handleChatMessage: ((msg: ChatMessage[]) => void) | null = null;

    static getInstance(): SignalRContext {
        if (!SignalRContext.instance) {
            SignalRContext.instance = new SignalRContext();
        }
        return SignalRContext.instance;
    }

    private constructor() {
        this.connection = new SignalR.HubConnectionBuilder()
            .withUrl("/hubs/session")
            .configureLogging(SignalR.LogLevel.Debug)
            .build();
        this.connection.onclose((err: Error | undefined) => {
            console.log("SignalR connection closed: " + err);
            this.connection
                .start()
                .then(() => this.onConnected())
                .catch((err: Error) => {
                    return console.log(err);
                });
        });
        this.connection.on("test", (data: string) => {
            console.log("recv " + data);
        });
        this.connection.on("Chat", (msg: ChatMessage[]) => {
            if (this.handleChatMessage !== null) {
                this.handleChatMessage(msg);
            }
        });
        this.connection.on("Scene", (sceneUpdate: Scene) => {
            if (this.handleSceneUpdate !== null) {
                this.handleSceneUpdate(sceneUpdate);
            } else {
                console.log("Stalling scene update: this.sceneUpdateHandler is null");
                this.lastSceneUpdate = sceneUpdate;
            }
        });
        this.connection
            .start()
            .then(() => this.onConnected())
            .catch((err: Error) => {
                return console.log(err);
            });
    }

    private onConnected() {
        if (this.sessionId !== null) {
            this.join();
        }
    }

    public setSceneUpdateHandler(handler: (s: Scene) => void) {
        console.log("setSceneUpdateHandler(" + handler + ")");
        this.handleSceneUpdate = handler;
        if (this.lastSceneUpdate !== null) {
            handler(this.lastSceneUpdate);
        }
    }
    public setChatMessageHandler(handler: (s: ChatMessage[]) => void) {
        console.log("setChatMessageHandler(" + handler + ")");
        this.handleChatMessage = handler;
    }

    public dragto(dx: number, dy: number) {
        console.log("Drag("+ dx + ", " + dy + ")");
        SignalRContext.getInstance().connection.send("Drag", dx, dy);
    }

    public join() {
        if (this.connection.state === SignalR.HubConnectionState.Connected) {
            console.log("Join(" + this.sessionId + ")");
            this.connection
                .send("Join", this.sessionId)
                .then(function () {
                    console.log("after joining...");
                });
        } else {
            console.log("join() aborting: HubConnection is not connected")
        }
    }
    public sendmsg(msg: string) {
        if (this.connection.state === SignalR.HubConnectionState.Connected) {
            console.log("sendmsg To " + this.sessionId + "("+msg+")");
            this.connection
                .send("Chat", msg)
                .then(function () {
                    console.log("after sendmsg...");
                });
        } else {
            console.log("sendmsg() aborting: HubConnection is not connected")
        }
    }

    public setSessionId(sessionId: number) {
        if (this.sessionId !== sessionId) {
            this.sessionId = sessionId;
            this.join();
        }
    }

    public ensureConnected() {
        console.log("##########################");
        console.log(this.connection.state);
        if (this.connection.state !== SignalR.HubConnectionState.Connected
            && this.connection.state !== SignalR.HubConnectionState.Connecting
            && this.connection.state !== SignalR.HubConnectionState.Reconnecting) {

            this.connection
                .start()
                .then(() => this.onConnected())
                .catch((err: Error) => {
                    return console.log(err);
                });
        }
    }
}
