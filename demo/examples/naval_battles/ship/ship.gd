@tool
class_name Ship
extends Node3D


@export var oscillation_deg := 10.0
@export var oscillation_max := 0.3
@export_enum("Large", "Skeleton Large") var skin_type := 0:
	get:
		return ship_skin.skin_type
	set(value):
		if value == null or ship_skin == null:
			return
		
		ship_skin.skin_type = value

@onready var cannon_left: ShipCannon = %CannonLeft
@onready var cannon_right: ShipCannon = %CannonRight
@onready var ship_skin: ShipSkin = %ShipSkin


var can_change_oscillation:= false
var oscillation_direction := 1


func _physics_process(delta: float) -> void:
	if Engine.is_editor_hint():
		return
	
	rotation.z  += deg_to_rad(oscillation_deg * delta) * oscillation_direction

	if absf(rotation.z) >= oscillation_max:
		oscillation_direction *= -1
