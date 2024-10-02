extends GPUParticles3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	one_shot = true
	
	finished.connect(func ():
		queue_free()	
	)
