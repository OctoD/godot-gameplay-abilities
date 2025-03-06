class_name MaxHealthAttribute
extends Attribute

const ATTRIBUTE_NAME := "MaxHealthAttribute"

func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	attribute_name = _attribute_name
	

func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME),
		attribute_set.find_by_name(StrengthAttribute.ATTRIBUTE_NAME)
	]


func _calculate(attributes: PackedFloat32Array) -> float:
	var constitution = attributes[0]
	var strength = attributes[1]

	return constitution * 4 + strength


func _get_buffed_value(values: PackedFloat32Array) -> float:
	return _calculate(values)
