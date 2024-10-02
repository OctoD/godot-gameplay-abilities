extends Node


@export var enemy_pool: Array[PackedScene] = []


var timer := Timer.new()


func _ready() -> void:
	add_child(timer)

	timer.one_shot = false
	timer.timeout.connect(spawn)
	timer.wait_time = 2.0

	timer.start()


func spawn() -> void:
	var enemy_scene = enemy_pool.pick_random() as PackedScene
	var enemy_instance = enemy_scene.instantiate()

	GameState.spawn_enemy(enemy_instance)
