class_name StaminaAttribute extends Attribute

const ATTRIBUTE_NAME = "StaminaAttribute"

func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = ATTRIBUTE_NAME
	
	
func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME)
	]

func _get_buffed_value(values: PackedFloat32Array) -> float:
	return values[0] * 2
	
	
func _get_initial_value(attribute_set: PackedFloat32Array) -> float:
	return attribute_set[0] * 2

	
func _get_max_value(attribute_set: AttributeSet) -> float:
	if attribute_set.has_attribute(ConstitutionAttribute.new()):
		return attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME).max_value * 2
	return 100.0


func _get_min_value(_attribute_set: AttributeSet) -> float:
	return 0
