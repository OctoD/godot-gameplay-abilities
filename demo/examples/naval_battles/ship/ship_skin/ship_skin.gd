@tool
class_name ShipSkin
extends Node3D

const SHIP_LARGE = preload("res://assets/kenney_pirate-kit/Models/GLB format/ship-large.glb")
const SHIP_PIRATE_LARGE = preload("res://assets/kenney_pirate-kit/Models/GLB format/ship-pirate-large.glb")


const skins: Array[PackedScene] = [
	SHIP_LARGE,
	SHIP_PIRATE_LARGE,
]

var __internal_skin_type := 1
@export_enum("Large", "Large Pirate") var skin_type := __internal_skin_type:
	get:
		return __internal_skin_type
	set(value):
		__internal_skin_type = value
		apply_skin(value)


func apply_skin(skin_number: int) -> void:
	assert(skin_number <= skins.size() - 1 && skin_number >= 0, "Invalid skin number")
	
	for child in get_children():
		child.queue_free()
		
	add_child(skins[skin_number].instantiate())
