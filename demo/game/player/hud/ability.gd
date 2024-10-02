@tool
class_name AbilityControl extends Control


@onready var progress_bar: ProgressBar = $ProgressBar
@onready var texture_rect: TextureRect = $TextureRect


@export var ability: AbilityWithIcon = null

var cooldown_time := 0.0
var cooldown_started := false

func _physics_process(delta: float) -> void:
	if cooldown_started:
		progress_bar.value -= delta


func _ready() -> void:
	if ability:
		texture_rect.texture = ability.icon


func bind_runtime_ability(runtime_ability: RuntimeAbility) -> void:
	runtime_ability.cooldown_start.connect(func ():
		progress_bar.value = runtime_ability.get_cooldown()
		progress_bar.max_value = runtime_ability.get_cooldown()
		progress_bar.min_value = 0.0
		cooldown_started = true
	)
	
	runtime_ability.cooldown_end.connect(func ():
		cooldown_started = false
		progress_bar.value = 0.0	
	)


func has_ability(runtime_ability: RuntimeAbility) -> bool:
	if ability == null:
		return false
		
	if runtime_ability == null:
		return false
	
	return runtime_ability.get_ability().ability_name == ability.ability_name
