class_name MovementSpeedAttribute extends Attribute

const ATTRIBUTE_NAME = "MovementSpeed"

func _init(_attribute_name = ATTRIBUTE_NAME) -> void:
	self.attribute_name = _attribute_name


func _calculate(attributes: PackedFloat32Array) -> float:
	var constitution = attributes[0]
	var dexterity = attributes[1]
	return constitution + (dexterity * 2)


func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(ConstitutionAttribute.ATTRIBUTE_NAME),
		attribute_set.find_by_name(DexterityAttribute.ATTRIBUTE_NAME)
	]


func _get_buffed_value(values: PackedFloat32Array) -> float:
	return _calculate(values)


func _get_initial_value(attribute_set: PackedFloat32Array) -> float:
	return _calculate(attribute_set)
