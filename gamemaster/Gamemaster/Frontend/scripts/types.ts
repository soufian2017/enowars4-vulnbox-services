export class Scene {
    units: { [id: string]: Unit } | null = null;
}

export class ChatMessage {
    id = 0;
    senderName = "SenderName";
    sessionContextId = 0;
    content = "Content";
    timestamp = "";
    timeString = "";
    tooltip = "";
}

export class Unit {
    x = 0;
    y = 0;
}
