import * as Phaser from 'phaser';
import { Scene, Unit } from "./types";
import { SignalRContext } from "./signalrhelper";

const sceneConfig: Phaser.Types.Scenes.SettingsConfig = {
    active: false,
    visible: false,
    key: 'Game'
};

export class Token extends Phaser.GameObjects.Sprite {
    constructor(scene: Phaser.Scene, x: number, y: number) {
        console.log("Token Created");
        super(scene, x, y, 'car', 0);
        this.setInteractive();
        scene.input.setDraggable(this);
        scene.input.on('dragend', function (_pointer: Phaser.Input.Pointer, gameObject: Token) {
            SignalRContext.getInstance().dragto(gameObject.x, gameObject.y);
        });
        scene.input.on('drag', (_pointer: Phaser.Input.Pointer, gameObject: Token, dragX: number, dragY: number) => {
            gameObject.x = dragX;
            gameObject.y = dragY;
        });
    }
}

export class CombatScene extends Phaser.Scene {
    private units: Map<string, Token> = new Map();

    constructor() {
        super(sceneConfig);
    }

    public preload() {
        console.log("CombatScene.preload");
        this.load.image('tiles', '/assets/tilemaps/tiles/tmw_desert_spacing.png');
        this.load.tilemapTiledJSON('map', '/assets/tilemaps/maps/desert.json');
        this.load.image('car', '/assets/sprites/car.png');
    }

    public create() {
        console.log("CombatScene.create");
        const map = this.make.tilemap({ key: 'map' });
        const tiles = map.addTilesetImage('Desert', 'tiles');
        const groundLayer = map.createDynamicLayer('Ground', tiles, 0, 0).setVisible(false);
        const rt = this.add.renderTexture(0, 0, 800, 600);
        rt.draw(groundLayer);
        SignalRContext.getInstance().setSceneUpdateHandler((s: Scene) => this.handleSceneUpdate(s));
    }

    private handleSceneUpdate(sceneUpdate: Scene) {
        console.log("handleSceneUpdate(" + JSON.stringify(sceneUpdate) + ")");
        for (const id in sceneUpdate.units) {
            const updatedUnit = sceneUpdate.units[id];
            const sprite = this.units.get(id);
            if (sprite !== undefined) {
                console.log("Updating sprite " + id);
                sprite.x = updatedUnit.x;
                sprite.y = updatedUnit.y;
            } else {
                console.log("Adding sprite " + id);
                const t = new Token(this, updatedUnit.x, updatedUnit.y);
                this.add.existing(t);
                this.units.set(id, t);
                //this.units.set(id, this.add.sprite(updatedUnit.x, updatedUnit.y, 'car', 0));
            }
        }
        this.units.forEach((_sprite: Phaser.GameObjects.Sprite, id: string) => {
            if (sceneUpdate.units !== null && sceneUpdate.units[id] === undefined) {
                const sprite = this.units.get(id);
                if (sprite !== undefined) {
                    sprite.destroy();
                }
                this.units.delete(id);
            }
        });
    }
}

