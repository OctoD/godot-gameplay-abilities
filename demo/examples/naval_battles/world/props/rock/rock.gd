@tool
extends Node3D

const ROCKS_A = preload("res://assets/kenney_pirate-kit/Models/GLB format/rocks-a.glb")
const ROCKS_B = preload("res://assets/kenney_pirate-kit/Models/GLB format/rocks-b.glb")
const ROCKS_C = preload("res://assets/kenney_pirate-kit/Models/GLB format/rocks-c.glb")
const ROCKS_SAND_A = preload("res://assets/kenney_pirate-kit/Models/GLB format/rocks-sand-a.glb")
const ROCKS_SAND_B = preload("res://assets/kenney_pirate-kit/Models/GLB format/rocks-sand-b.glb")
const ROCKS_SAND_C = preload("res://assets/kenney_pirate-kit/Models/GLB format/rocks-sand-c.glb")
const SHIP_WRECK = preload("res://assets/kenney_pirate-kit/Models/GLB format/ship-wreck.glb")

const ROCKS_ARRAY: Array[PackedScene] = [
	ROCKS_A,
	ROCKS_B,
	ROCKS_C,
	ROCKS_SAND_A,
	ROCKS_SAND_B,
	ROCKS_SAND_C,
	SHIP_WRECK,
]

@export_tool_button("Randomize rock", "Callable") var set_rand_rock = randomize_rock
@export var rock_scene: PackedScene

@onready var collision_shape_3d: CollisionShape3D = %CollisionShape3D
@onready var node_3d: Node3D = %Mesh


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	if rock_scene && !Engine.is_editor_hint():
		node_3d.add_child(rock_scene.instantiate())


func randomize_rock() -> void:
	randomize()
	
	for child in node_3d.get_children():
		child.queue_free()

	rock_scene = ROCKS_ARRAY.pick_random()

	var box_shape = BoxShape3D.new()
	var new_rock = rock_scene.instantiate() as Node3D
	var mesh = new_rock.get_child(0)
	var size = Vector3.ONE * randf_range(0.5, 30.0)

	node_3d.add_child(new_rock)
	mesh.get_aabb().size = size
	box_shape.size = size
	collision_shape_3d.shape = box_shape
	new_rock.rotation.y = randf_range(0.0, 1.0)

	notify_property_list_changed()
