extends Node


var pawns: Array[AiControlled] = []
var players: Array[Player] = []
var started := false


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	if !started:
		return

	var index = 0

	for ai in pawns:
		ai.acquire_target(players)
		ai.move()
		ai.attack()
		
		if ai.controlled_enemy == null || ai.controlled_enemy.is_queued_for_deletion():
			pawns.remove_at(index)
			
		index += 1

func control_enemy(enemy: Enemy) -> void:
	var agent = AiControlled.new()
	agent.controlled_enemy = enemy
	pawns.append(agent)
	#enemy.position = NavigationServer3D.region_get_random_point(agent.get_navigation_map(), 1, false)


func start_game(maybe_players: Array[Node]) -> void:
	started = true
	
	for x in maybe_players:
		if x is Player:
			players.append(x)


class AiControlled extends NavigationAgent3D:
	var aggro: Player
	var controlled_enemy: Enemy

		
	func acquire_target(players: Array[Player]) -> void:
		aggro = nearest_player(players)


	func attack() -> void:
		if controlled_enemy == null:
			return
		
		if controlled_enemy.ability_container != null and controlled_enemy.ability_container.has_ability(FireProjectileAbility.ABILITY_NAME):
			controlled_enemy.ability_container.try_activate(FireProjectileAbility.ABILITY_NAME)


	func nearest_player(players: Array[Player]) -> Player:
		if players.size() == 0:
			return null
		
		var copy = players.duplicate()
		copy.sort_custom(sort_closest)
		return copy.front()


	func move() -> void:
		if aggro == null:
			return
			
		if controlled_enemy == null:
			return

		controlled_enemy.look_at(aggro.position)
		controlled_enemy.aggro_direction = controlled_enemy.global_position.normalized() + aggro.global_position.normalized()


	func sort_closest(a: Node3D, b: Node3D):
		return a.position < b.position
