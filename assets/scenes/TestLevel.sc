{
    "name": "TestScene",
    "objects":
    [
        {
            "name": "MainPlayer",
            "type": "Player",
            "position": 
            {
                "x": 0,
                "y": 2,
                "z": 7
            },
            "components":
            [
                { "type": "CameraComponent" },
                { "type": "PlayerControllerComponent" },
                { "type": "AudioListenerComponent" },
                { 
                    "type": "AudioComponent",
                    "audio":
                    [
                        { "name": "shoot", "filepath": "audio/shoot.wav" },
                        { "name": "jump", "filepath": "audio/jump.wav" },
                        { "name": "step", "filepath": "audio/step.wav"}
                    ]
                }
            ],
            "children":
            [
                {
                    "name": "Gun",
                    "type": "gltf",
                    "path": "models/sten_gunmachine_carbine/scene.gltf",
                    "position": { "x": 0.75, "y": -0.5, "z": -0.75 },
                    "scale": { "x": 1, "y": 1, "z": 1 }
                }
            ]
        },
        {
            "name": "Ground",
            "position": 
            {
                "x" : 0,
                "y" : 0,
                "z" : 0
            },
            "components":
            [
                { 
                    "type": "MeshComponent",
                    "mesh":
                    {
                        "type": "box",
                        "x": 50,
                        "y": 2,
                        "z": 50
                    },
                    "material":
                    {
                        "path": "materials/checker.mat"
                    }
                },
                {
                    "type": "PhysicsComponent",
                    "collider": 
                    {
                        "type": "box",
                        "x": 50,
                        "y": 2,
                        "z": 50
                    },
                    "body" : 
                    {
                        "mass": 0,
                        "friction": 0.5,
                        "type": "static"
                    }
                }
            ]
        },
        {
            "name": "JumpPlatform",
            "type": "JumpPlatform",
            "position":
            {
                "x": 5,
                "y": 2,
                "z": 5
            },
            "components":
            [
                { 
                    "type": "MeshComponent",
                    "mesh":
                    {
                        "type": "box",
                        "x": 2,
                        "y": 0.2,
                        "z": 2
                    },
                    "material":
                    {
                        "path": "materials/checker.mat",
                        "params":
                        {
                            "float3":
                            [
                                { "name": "color", "value0": 1, "value1": 0, "value2": 0 }
                            ]
                        }
                    }
                },
                {
                    "type": "PhysicsComponent",
                    "collider": 
                    {
                        "type": "box",
                        "x": 2,
                        "y": 0.2,
                        "z": 2
                    },
                    "body" : 
                    {
                        "mass": 0,
                        "friction": 0.5,
                        "type": "static"
                    }
                }
            ]
        },
        {
            "name": "Wall_Front",
            "position": 
            {
                "x" : 0,
                "y" : 3,
                "z" : -26
            },
            "components":
            [
                { 
                    "type": "MeshComponent",
                    "mesh":
                    {
                        "type": "box",
                        "x": 50,
                        "y": 4,
                        "z": 2
                    },
                    "material": 
                    {
                        "path": "materials/checker.mat"
                    }
                },
                {
                    "type": "PhysicsComponent",
                    "collider": 
                    {
                        "type": "box",
                        "x": 50,
                        "y": 4,
                        "z": 2
                    },
                    "body" : 
                    {
                        "mass": 0,
                        "friction": 0.5,
                        "type": "static"
                    }
                }
            ]
        },       
        {
            "name": "Light",
            "position":
            {
                "x": 0,
                "y": 5,
                "z": 0
            },
            "components":
            [
                {
                    "type": "LightComponent",
                    "color": 
                    { 
                        "r": 1,
                        "g": 1,
                        "b": 1
                    }
                }
            ]
        }    
    ],
    "camera": "MainPlayer"
}
