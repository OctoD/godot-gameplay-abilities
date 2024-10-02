# godot-gameplay-abilities

![GGAS logo](godot/icon.svg)

> (Part of [Godot Gameplay Systems](https://github.com/OctoD/godot-gameplay-systems))

# Installation

## Using the AssetLib

1. Open the Godot editor.
2. Navigate to the AssetLib tab.
3. Search for "Gameplay Abilities".
4. Install and enjoy!


## Using the GitHub repository

1. Download the latest version of the addon from the [releases page](https://github.com/OctoD/godot-gameplay-abilities/releases).
2. Extract the contents of the zip file into your project's `addons` folder.
3. Enjoy!

## Using Git

1. Clone the repository into your project's `addons` folder:

```bash
git clone --recurse-submodules
```

2. Build the project:

```bash
scons platform=windows # or linux, macos etc
```
3. Enjoy!

# Usage

This addon provides a set of nodes and tools to create gameplay abilities in Godot. It is designed to be flexible and easy to use, while providing a solid foundation for more complex systems.

This system relies on three things:

- `Ability`: A single ability that can be activated, granted, revoked, etc. Works like a blueprint of an ability.
- `AbilityContainer`: A container that holds abilities. It can be attached to any node in the scene.
- `RuntimeAbility`: The runtime representation of an ability. It is created when an ability is added to an `AbilityContainer`.

Each ability, in order to be used, must be granted first. 

The grant can occur automatically if the overridden method `_can_be_granted` returns true or if it is not overridden, or it can be done manually by calling the `try_grant` method of the `AbilityContainer`.

A granted ability can be revoked by calling the `try_revoke` method of the `AbilityContainer`.

## Creating an ability

An ability is nothing more than a scripted `Resource` that extends the `Ability` class. You can create a new ability by creating a new script and extending the `Ability` class:

```gd
# my_ability.gd
class_name MyAbility extends Ability

# The name of the ability, used to identify it
const ABILITY_NAME := "MyAbility"

func _init(_ability_name = ABILITY_NAME):
    ability_name = _ability_name

#
# Each ability has several virtual methods which can be overridden to customize its behavior:
#

# Determines if the ability can activate it's cooldown
func _can_activate_cooldown(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> bool:
    return true

# Determines if the ability can be activated
func _can_be_activated(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> bool:
    return true

# Determines if the ability can be blocked
func _can_be_blocked(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> bool:
    pass

# Determines if the ability can be granted
func _can_be_granted(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> bool:
    pass

# Determines if the ability can be revoked
func _can_be_revoked(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> bool:
    pass

# Determines if the ability can be ended
func _can_be_ended(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> bool:
    pass

# Gets the cooldown duration of the ability
_get_cooldown(ability_container: AbilityContainer) -> float:
    return 0.0

# Gets the ability duration. Once this duration is over, the ability will try to end itself if possible (based on other methods).
_get_duration(ability_container: AbilityContainer) -> float:
    return 0.0

# Called when the ability is activated, useful to do things in the 2d or 3d world, launch other abilities, animations, etc.
_on_activate(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> void:
    pass

# Called when the ability is blocked, ...
_on_block(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> void:
    pass

# Called when the ability is granted, ...
_on_end(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> void:
    pass

# Called when the ability is granted, ...
_on_grant(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> void:
    pass

# Called when the ability is revoked, ...
_on_revoke(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> void:
    pass

# Called for each tick of the _physics_process of the AbilityContainer
_on_tick(delta: float, cooldown_time: float, ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> void:
    pass

# Called for each tick of the _process of the AbilityContainer, if it returns true, the ability will be activated again
_should_be_activated(ability_container: AbilityContainer) -> bool:
    return false

# Called for each tick of the _process of the AbilityContainer, if it returns true, the ability will be blocked
_should_be_blocked(ability_container: AbilityContainer) -> bool:
    return false

# Called for each tick of the _process of the AbilityContainer, if it returns true, the ability will be granted
_should_be_ended(ability_container: AbilityContainer) -> bool:
    return false
```

## Adding an ability to an AbilityContainer

You can add an ability to an `AbilityContainer` in two ways:

- Using the editor: Select the `AbilityContainer` node, navigate to the `Abilities` property and add a new element. Then, select the ability you want to add.
- Programmatically: You can add an ability to an `AbilityContainer` using the `add_ability` method:

```gd
# Add an ability to the container
$AbilityContainer.add_ability(MyAbility.new())
```

## Contributing

If you want to contribute to this project, please read the [CONTRIBUTING.md](CONTRIBUTING.md) file.

Don't be shy, this project is open to any kind of contribution!

## License

[MIT](LICENSE)