extends Node


var enemies_spawn_point: Node3D
var navigation_region_3d: NavigationRegion3D
var projectiles_spawn_point: Node3D
var maximum_enemies_count := 100


func spawn_enemy(enemy: Enemy) -> void:
	if AiPuppeteer.pawns.size() < maximum_enemies_count && enemies_spawn_point:	
		enemies_spawn_point.add_child(enemy)
		return AiPuppeteer.control_enemy(enemy)


func spawn_projectile(projectile: Node3D) -> void:
	if projectiles_spawn_point:
		projectiles_spawn_point.add_child(projectile)
