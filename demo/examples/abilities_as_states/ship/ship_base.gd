class_name ShipBase
extends RigidBody3D


@export_category("Maneuvering")
@export_group("Base values", "base_")
@export var base_acceleration   := 0.015
@export_group("Max values", "max_")
@export var max_acceleration 	:= 0.25
@export var max_speed 			:= 20.0
@export var max_steering		:= 15.0

@export_category("Ships state")
@export var max_anchor_down		:= 1.0

@export_category("Ship instance™️")
@export var ship: Ship			= null


var acceleration	:= 0.0
var anchor_down		:= 0.0
var cannoballs		:= 40
var health			:= 100.0
var movement_speed  := 0.0
var wheel_steer		:= 0.0


func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	state.linear_velocity = global_transform.basis * Vector3(0, 0, -movement_speed) 
	state.apply_force(Vector3(0.0, deg_to_rad(wheel_steer) * 0.5, 0.0))
	movement_speed = clampf(movement_speed + acceleration, -max_speed, max_speed)
	print(movement_speed)


func sail(delta: float) -> void:
	rotation.y += deg_to_rad(wheel_steer * delta) * 0.5


func accelerate(p_accelerate: float) -> void:
	acceleration = clampf(acceleration + (p_accelerate * base_acceleration), -0.2, max_acceleration)
	linear_velocity = global_transform.basis * Vector3(0, 0, -acceleration) 


func steer(angle: float) -> void:
	wheel_steer += angle
	wheel_steer = clampf(wheel_steer, -max_steering, max_steering)
