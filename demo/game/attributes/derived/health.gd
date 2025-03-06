class_name HealthAttribute extends Attribute

const ATTRIBUTE_NAME = "Health"

func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	self.attribute_name = _attribute_name
	
	
func _constrained_by(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(MaxHealthAttribute.ATTRIBUTE_NAME),
	]


func _calculate(attributes: PackedFloat32Array) -> float:
	var constitution = attributes[0]
	var strength = attributes[1]

	return constitution * 4 + strength


func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME),
		attribute_set.find_by_name(StrengthAttribute.ATTRIBUTE_NAME)
	]


func _get_buffed_value(values: PackedFloat32Array) -> float:
	return _calculate(values)
	
	
func _get_constrained_value(buffed_value: float, buffed_values: PackedFloat32Array, previous_values: PackedFloat32Array) -> float:
	return clampf(buffed_value, 0, buffed_values[0])


func _get_initial_value(attribute_set: PackedFloat32Array) -> float:
	return _calculate(attribute_set)
