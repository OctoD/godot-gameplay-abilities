class_name ShipAi
extends Node


enum States {
	ROAM,
	AGGRO
}


@export var ship: ShipBase		= null
@export var max_aggro_distance := 50.0


var aggroed_player: NavalBattlesPlayer
var state: int = 0


func _process(_delta: float) -> void:
	match state:
		States.AGGRO: handle_aggro()
		States.ROAM:  handle_roam()


func _physics_process(delta: float) -> void:
	if ship:
		ship.sail(delta)


func handle_aggro() -> void:
	scan_for_aggro()
	move_towards_aggro()
	
	
func handle_roam() -> void:
	scan_for_aggro()


func move_towards_aggro() -> void:
	pass


func scan_for_aggro() -> void:
	pass
