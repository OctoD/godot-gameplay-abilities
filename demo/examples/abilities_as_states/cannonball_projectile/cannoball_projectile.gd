extends Area3D


var direction	:= Vector3.ZERO
var speed 		:= 10.0
var velocity 	:= Vector3.ZERO


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass
	#body_entered.connect(func (collided_body: Node3D):
		#queue_free()	
	#)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	#velocity.y -= delta * gravity
	velocity.z += delta * speed 


func shoot_at(p_direction: Vector3) -> void:
	direction + p_direction
	look_at(direction)
	velocity.y += 10.0
