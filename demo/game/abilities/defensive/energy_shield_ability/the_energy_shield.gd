extends Node3D


@onready var animation_player: AnimationPlayer = $AnimationPlayer



func _ready() -> void:
	set_meta("barrier", true)
	
	animation_player.animation_finished.connect(func (animation_name: StringName):
		if animation_name == "deactivated":
			queue_free()	
	)
