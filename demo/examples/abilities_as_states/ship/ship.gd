@tool
class_name Ship
extends Node3D


@export var oscillation_deg := 10.0
@export var oscillation_max := 0.3

@onready var cannon_left: Node3D = %cannon_left
@onready var cannon_right: Node3D = %cannon_right


var can_change_oscillation:= false
var oscillation_direction := 1


func _physics_process(delta: float) -> void:
	var sign = 0
	
	rotation.z  += deg_to_rad(oscillation_deg * delta) * oscillation_direction

	if absf(rotation.z) >= oscillation_max:
		oscillation_direction *= -1
