class_name StaminaAttribute extends Attribute

const ATTRIBUTE_NAME = "StaminaAttribute"

func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = ATTRIBUTE_NAME


func _constrained_by(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME)
	]
	
func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME)
	]

func _get_buffed_value(values: PackedFloat32Array) -> float:
	return values[0] * 2
	
	
func _get_initial_value(attribute_set: PackedFloat32Array) -> float:
	return attribute_set[0] * 2


func _get_constrained_value(buffed_value: float, buffed_values: PackedFloat32Array, _previous_values: PackedFloat32Array) -> float:
	return clampf(buffed_value, 0.0, buffed_values[0])
