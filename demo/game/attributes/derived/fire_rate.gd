class_name FireRateAttribute extends Attribute


const ATTRIBUTE_NAME = "FireRateAttribute"


const MAX_COOLDOWN := 2.0
const MIN_COOLDOWN := 0.9

const DEXTERITY_MULTIPLIER := 0.2
const INTELLIGENCE_MULTIPLIER := 0.1


func calculate(values: PackedFloat32Array) -> float:
	var intelligence = values[0]
	var dexterity = values[1]

	return clampf(MAX_COOLDOWN - ((intelligence * INTELLIGENCE_MULTIPLIER) + (dexterity * DEXTERITY_MULTIPLIER)), MIN_COOLDOWN, MAX_COOLDOWN)


func _init(_attribute_name := ATTRIBUTE_NAME):
	attribute_name = _attribute_name


func _derived_from(attribute_set: AttributeSet) -> Array[AttributeBase]:
	return [
		attribute_set.find_by_name(IntelligenceAttribute.ATTRIBUTE_NAME),
		attribute_set.find_by_name(DexterityAttribute.ATTRIBUTE_NAME)
	]


func _get_buffed_value(values: PackedFloat32Array) -> float:
	return calculate(values)
	
	
func _get_initial_value(values: PackedFloat32Array) -> float:
	return calculate(values)

	
func _get_max_value(_attribute_set: AttributeSet) -> float:
	return MAX_COOLDOWN # seconds in cooldown


func _get_min_value(_attribute_set: AttributeSet) -> float:
	return 0.9 # seconds in cooldown
