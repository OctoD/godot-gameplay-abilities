extends Node

@export var enemies: Array[PackedScene] = []
@export var map: Node3D = null
@export var max_enemies_count := 32
@export var map_size := 1024


func _handle_timer_timeout() -> void:
	if enemies.size() == 0 || map == null:
		return

	var cell_size = map_size / 2

	for i in range(map.get_child_count(), max_enemies_count):
		randomize()
		var new_enemy = enemies.pick_random().instantiate()
		map.add_child(new_enemy)
		map.global_position = Vector3(randf_range(-cell_size, cell_size), 0, randf_range(-cell_size, cell_size))


func _ready() -> void:
	var timer: = Timer.new()

	add_child(timer)

	timer.timeout.connect(_handle_timer_timeout)
	timer.wait_time = 5.0
	timer.start()
