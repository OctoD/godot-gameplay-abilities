extends Control

@export var examples: Array[PackedScene] = []


@onready var main_menu: CenterContainer = %MainMenu
@onready var main_menu_content: VBoxContainer = %MainMenuContent
@onready var back_to_main_menu_button: Button = %BackToMainMenuButton
@onready var example_menu: HBoxContainer = %ExampleMenu
@onready var example_sub_viewport: SubViewport = %ExampleSubViewport
@onready var examples_container: VBoxContainer = $MainMenu/MainMenuContent/ExamplesContainer
@onready var quit_button: Button = %QuitButton


func _ready() -> void:
	example_menu.hide()
	
	quit_button.pressed.connect(func () -> void:
		get_tree().quit(0)
	)
	
	back_to_main_menu_button.pressed.connect(func () -> void:
		for child in example_sub_viewport.get_children():
			child.queue_free()
		example_menu.hide()
		main_menu.show()
	)
	
	for example in examples:
		var button = Button.new()
		var chunks = example.resource_path.split("/")
		var example_name = chunks[chunks.size() - 1].replace(".tscn", "").replace("_", " ")
		
		button.text = chunks[chunks.size() - 1].replace(".tscn", "").replace("_", " ").capitalize()
		button.text
		button.pressed.connect(func ():
			main_menu.hide()
			example_menu.show()
			example_sub_viewport.add_child(example.instantiate())
		)
		examples_container.add_child(button)
