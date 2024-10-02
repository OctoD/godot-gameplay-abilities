extends Node3D

@onready var enemies: Node3D = %Enemies
@onready var player: Node3D = %Player
@onready var projectiles: Node3D = %Projectiles
@onready var navigation_region_3d: NavigationRegion3D = $NavigationRegion3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	GameState.enemies_spawn_point = enemies
	GameState.projectiles_spawn_point = projectiles
	GameState.navigation_region_3d = navigation_region_3d
	
	AiPuppeteer.start_game(player.get_children())


func _input(event: InputEvent) -> void:
	if event is InputEventKey:
		if event.as_text_keycode() == "Escape":
			Input.mouse_mode = Input.MOUSE_MODE_VISIBLE

	if event is InputEventMouseButton && Input.mouse_mode == Input.MOUSE_MODE_VISIBLE: 
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
