extends Area3D


var direction			:= Vector3.ZERO
var g 					:= Vector3.DOWN * 3.2
var speed 				:= 50.0
var shooted_by:	Node3D 	 = null
var velocity 			:= Vector3.ZERO


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	body_entered.connect(func (collided_body: Node3D):
		if collided_body != shooted_by:
			var maybe_ship = collided_body as ShipBase
			
			if maybe_ship != null:
				maybe_ship.ability_container.try_activate(NavalBattlesTakeDamage.new())
			
			queue_free()	
	)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	velocity += g * delta
	look_at(transform.origin + velocity.normalized(), Vector3.UP)
	transform.origin += velocity * delta
