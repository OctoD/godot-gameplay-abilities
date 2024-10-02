class_name JumpHeightAttribute extends Attribute

const ATTRIBUTE_NAME = "JumpHeight"

func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	self.attribute_name = _attribute_name


func _calculate(attributes: PackedFloat32Array) -> float:
	var dexterity = attributes[0]
	var strength = attributes[1]

	return dexterity / 2 + strength / 1.2


func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(DexterityAttribute.ATTRIBUTE_NAME),
		attribute_set.find_by_name(StrengthAttribute.ATTRIBUTE_NAME)
	]


func _get_buffed_value(values: PackedFloat32Array) -> float:
	return _calculate(values)


func _get_initial_value(attribute_set: PackedFloat32Array) -> float:
	return _calculate(attribute_set)
