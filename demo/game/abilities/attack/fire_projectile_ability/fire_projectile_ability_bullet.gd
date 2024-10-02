class_name FireProjectileAbilityBullet extends Area3D


const FIRE_PROJECTILE_EXPLOSION = preload("res://game/abilities/attack/fire_projectile_ability/fire_projectile_explosion.tscn")


var direction: Vector3 = Vector3.ZERO
var projectile_speed: float = 40.0


func _ready() -> void:
	get_tree().create_timer(10.0).timeout.connect(_on_timer_timeout)
	body_entered.connect(_on_body_entered)


func _physics_process(delta: float) -> void:
	global_transform.origin -= transform.basis.z.normalized() * projectile_speed * delta


func _on_timer_timeout() -> void:
	self.queue_free()


func _on_body_entered(body: Node3D) -> void:
	var instance = FIRE_PROJECTILE_EXPLOSION.instantiate()

	GameState.projectiles_spawn_point.add_child(instance)
	
	instance.global_transform = global_transform
	instance.global_position = global_position

	if body.has_meta("barrier"):
		queue_free()
		return

	if body is CharacterBase:
		var health_debuff = AttributeBuff.new()
		health_debuff.attribute_name = HealthAttribute.ATTRIBUTE_NAME
		health_debuff.operation = AttributeOperation.subtract(10)
		body.attribute_container.apply_buff(health_debuff)

	queue_free()
