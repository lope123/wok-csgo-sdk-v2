#pragma once

class ik_context {
public:
	CUSTOM_VFUNC(init(void* hdr, qangle_t& angles, vec3_t& origin, float time, int framecount, int mask), "client_panorama.dll", "55 8B EC 83 EC ? 8B 45 ? 56 57 8B F9 8D 8F ? ? ? ?",
		void(__thiscall*)(void*, void*, qangle_t&, vec3_t&, float, int, int), hdr, angles, origin, time, framecount, mask)

	CUSTOM_VFUNC(update_targets(vec3_t* pos, quaternion* q, matrix3x4_t* bones, uint8_t* computed), "client_panorama.dll", "55 8B EC 83 E4 ? 81 EC ? ? ? ? 33 D2",
		void(__thiscall*)(void*, vec3_t*, void*, matrix3x4_t*, uint8_t*), pos, q, bones, computed)

	CUSTOM_VFUNC(solve_dependencies(vec3_t* pos, quaternion* q, matrix3x4_t* bones, uint8_t* computed), "client_panorama.dll", "55 8B EC 83 E4 ? 81 EC ? ? ? ? 8B 81 ? ? ? ? 56",
		void(__thiscall*)(void*, vec3_t*, void*, matrix3x4_t*, uint8_t*), pos, q, bones, computed)

	void clear_targets() {
		auto i = 0;
		auto count = *reinterpret_cast<int*>(uintptr_t(this) + 4080);

		if (count > 0) {
			auto target = reinterpret_cast<int*>(uintptr_t(this) + 208);
			do {
				*target = -9999;
				target += 85;
				++i;
			} while (i < count);
		}
	}
};

class c_bone_setup {
public:
	CUSTOM_VFUNC(accumulate_pose(vec3_t* pos, quaternion* q, int sequence, float cycle, float weight, float time, ik_context* ik),
		"client_panorama.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? A1", void(__thiscall*)(void*, vec3_t*, quaternion*, int, float, float, float, ik_context*), pos, q, sequence, cycle, weight, time, ik)
};

class c_base_entity : public i_client_entity {
public:
	VFUNC(get_pred_desc_map(), 17, datamap_t*(__thiscall*)(void*))

	NETVAR(did_smoke_effect(), bool, "CSmokeGrenadeProjectile->m_bDidSmokeEffect")
	NETVAR(get_last_made_noise_time(), float, "CBaseEntity->m_flLastMadeNoiseTime")

	NETVAR(get_rotation(), qangle_t, "CBaseEntity->m_angRotation")
	NETVAR(get_team(), team_id, "CBaseEntity->m_iTeamNum")
	NETVAR(get_origin(), vec3_t, "CBaseEntity->m_vecOrigin")
	NETVAR(get_owner_entity(), c_base_handle , "CBaseEntity->m_hOwnerEntity")
	NETVAR(get_thrower_handle(), c_base_handle, "CBaseGrenade->m_hThrower");
	NETVAR(get_model_index(), int, "CBaseEntity->m_nModelIndex")
	NETVAR(is_spotted(), bool, "CBaseEntity->m_bSpotted")

	NETVAR(get_simulation_time(), float, "CBaseEntity->m_flSimulationTime")
	NETVAR_OFFSET(get_old_simulation_time(), float, "CBaseEntity->m_flSimulationTime", 0x4)
	NETVAR_OFFSET(get_rgfl_coordinate_frame(), matrix3x4_t, "CBaseEntity->m_CollisionGroup", -0x30)

	VFUNC(get_abs_angles(), 11, qangle_t&(__thiscall*)(void*))
	VFUNC(get_abs_origin(), 10, vec3_t&(__thiscall*)(void*))

	VFUNC(is_player(), 157, bool(__thiscall*)(void*))
	VFUNC(is_weapon(), 165, bool(__thiscall*)(void*))
	VFUNC(set_model_index(int id), 75, void(__thiscall*)(void*, int), id)

	CUSTOM_VFUNC(set_abs_angles(qangle_t angles), "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1", void(__thiscall*)(void*, const qangle_t&), angles)
	CUSTOM_VFUNC(set_abs_origin(vec3_t origin), "client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1", void(__thiscall*)(void*, const vec3_t&), origin)

	OFFSET(get_renderable(), i_client_renderable*, 0x4)
	OFFSET(get_networkable(), i_client_networkable*, 0x8)

	OFFSET(get_model_hdr(), studiohdr_t*, 0x294C)
	POFFSET(get_bone_cache(), c_bone_cache, 0x290C + 0x4)
	OFFSET(get_occlusion_mask(), int, 0xA24)
	OFFSET(get_occlusion_framecount(), int, 0xA30)
	OFFSET(get_unknown_occlusion_flags(), int, 0xA2C)
	OFFSET(get_occlusion_flags(), int, 0xA28)
	OFFSET(get_bone_array_for_write(), matrix3x4_t*, 0x26A8)
	OFFSET(get_last_setup_bones_framecount(), int, 0xA64 + 0x4)
	OFFSET(get_predictable(), int, 0x2EA + 0x4)
	OFFSET(get_accumulated_bone_mask(), int, 0x269C + 0x4)
	OFFSET(get_prev_bone_mask(), int, 0x2698 + 0x4)
	OFFSET(get_readable_bones(), int, 0x26A8 + 0x4)
	OFFSET(get_writable_bones(), int, 0x26AC + 0x4)
	OFFSET(get_most_recent_model_bone_counter(), unsigned int, 0x268C + 0x4)
	OFFSET(get_last_setup_bones_time(), unsigned int, 0x2920 + 0x4)
	OFFSET(get_ik_context(), ik_context*, 0x266C + 0x4)
	OFFSET(get_setup_bones_pos(), vec3_t, 0xA68 + 0x4)
	OFFSET(get_setup_bones_quaternion(), quaternion, 0x166C + 0x4)
	OFFSET(get_take_damage(), int, 0x280)

	DATAMAP(get_effects(), int, "m_fEffects")
	DATAMAP(get_eflags(), int, "m_iEFlags")
	DATAMAP(get_abs_velocity(), vec3_t, "m_vecAbsVelocity")
	DATAMAP(get_abs_rotation(), qangle_t, "m_angAbsRotation")

	void invalidate_bone_cache() {
		static const auto addr = SIG("client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
		auto g_iModelBoneCounter = **reinterpret_cast<unsigned long**>(addr + 0xA);
		get_last_setup_bones_time() = 0xFF7FFFFF;
		get_most_recent_model_bone_counter() = g_iModelBoneCounter - 1;
	}

	bool is_enemy();

	bool is_breakable() {
		if (!this)
			return false;

		auto backup_take_damage = get_take_damage();
		static const auto is_breakable_fn = reinterpret_cast<bool(__thiscall*)(void*)>(SIG("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68"));

		auto class_id = get_client_class()->m_class_id;
		auto is_breakable_class = class_id == CBaseDoor
			|| class_id == CBreakableSurface || class_id == CFuncBrush
			|| class_id == CBaseEntity && get_collideable()->get_solid() == SOLID_BSP;

		is_breakable_class ? get_take_damage() = DAMAGE_YES : 0;
		auto ret = is_breakable_fn(this);
		is_breakable_class ? get_take_damage() = backup_take_damage : 0;

		return ret;
	}
};

class c_base_view_model : public c_base_entity {
public:
	NETVAR(get_owner(), c_base_handle, "CBaseViewModel->m_hOwner")
	NETVAR(get_weapon(), c_base_handle, "CBaseViewModel->m_hWeapon")
	NETPROP(get_sequence_prop(), "CBaseViewModel->m_nSequence")
};

class c_base_weapon_world_model : public c_base_entity {
public:

};

class c_planted_c4 : public c_base_entity {
public:
	NETVAR(is_ticking(), bool, "CPlantedC4->m_bBombTicking")
	NETVAR(get_site(), int, "CPlantedC4->m_nBombSite")
	NETVAR(get_c4_blow(), float, "CPlantedC4->m_flC4Blow")
	NETVAR(get_timer_length(), float, "CPlantedC4->m_flTimerLength")
	NETVAR(get_defuse_length(), float, "CPlantedC4->m_flDefuseLength")
	NETVAR(get_defuse_countdown(), float, "CPlantedC4->m_flDefuseCountDown")
	NETVAR(is_defused(), bool, "CPlantedC4->m_bBombDefused")
	NETVAR(get_defuser(), c_base_handle, "CPlantedC4->m_hBombDefuser")
};

class c_base_attributable_item : public c_base_entity {
public:
	NETVAR(get_item_definition_index(), short, "CBaseAttributableItem->m_iItemDefinitionIndex")
	NETVAR(get_account_id(), int, "CBaseAttributableItem->m_iAccountID")
	NETVAR(get_item_quality(), int, "CBaseAttributableItem->m_iEntityQuality")
	NETVAR(get_item_id_high(), int, "CBaseAttributableItem->m_iItemIDHigh")
	NETVAR(get_item_id_low(), int, "CBaseAttributableItem->m_iItemIDLow")
	NETVAR(get_fallback_paint_kit(), int, "CBaseAttributableItem->m_nFallbackPaintKit")
	NETVAR(get_fallback_seed(), int, "CBaseAttributableItem->m_nFallbackSeed")
	NETVAR(get_fallback_wear(), int, "CBaseAttributableItem->m_flFallbackWear")
	NETVAR(get_fallback_stat_trak(), int, "CBaseAttributableItem->m_nFallbackStatTrak")
};

class c_base_combat_weapon;

class c_base_combat_character : public c_base_entity {
public:
	PNETVAR(get_weapons(), c_base_handle, "CBaseCombatCharacter->m_hMyWeapons")
	PNETVAR(get_wearables(), c_base_handle, "CBaseCombatCharacter->m_hMyWearables")
	NETVAR(get_active_weapon_handle(), c_base_handle, "CBaseCombatCharacter->m_hActiveWeapon")

	c_base_combat_weapon* get_active_weapon();
};

class c_base_player : public c_base_combat_character {
public:
	DATAMAP(get_duck_amount(), float, "m_flDuckAmount")
	DATAMAP(get_ground_entity(), c_base_handle, "m_hGroundEntity")
	DATAMAP(get_move_type(), int, "m_MoveType")
	DATAMAP(get_next_attack(), float, "m_flNextAttack")

	DATAMAP(get_impulse(), int, "m_nImpulse")
	DATAMAP(get_buttons(), int, "m_nButtons")
	DATAMAP(get_buttons_last(), int, "m_afButtonLast")
	DATAMAP(get_buttons_pressed(), int, "m_afButtonPressed")
	DATAMAP(get_buttons_released(), int, "m_afButtonReleased")

	DATAMAP(get_collision_state(), int, "m_vphysicsCollisionState")

	OFFSET(get_spawn_time(), float, 0xA360)

	NETVAR(get_fall_velocity(), float, "CBasePlayer->m_flFallVelocity")
	NETVAR(get_observer_mode(), observer_mode, "CBasePlayer->m_iObserverMode")
	NETVAR(get_observer_target(), c_base_handle, "CBasePlayer->m_hObserverTarget")
	NETVAR(get_flags(), int, "CBasePlayer->m_fFlags")
	NETVAR(get_velocity(), vec3_t, "CBasePlayer->m_vecVelocity[0]")
	NETVAR(get_vehicle(), c_base_handle, "CBasePlayer->m_hVehicle")
	NETVAR(get_water_level(), int, "CBasePlayer->m_nWaterLevel")
	NETVAR(get_next_think_tick(), int, "CBasePlayer->m_nNextThinkTick")
	NETVAR(get_tickbase(), int, "CBasePlayer->m_nTickBase")
	NETVAR(get_duck_speed(), float, "CBasePlayer->m_flDuckSpeed")
	NETVAR(get_view_offset(), vec3_t, "CBasePlayer->m_vecViewOffset[0]")
	NETVAR(get_health(), int, "CBasePlayer->m_iHealth")
	NETVAR(get_life_state(), life_state, "CBasePlayer->m_lifeState")
	NETVAR(get_view_punch_angle(), qangle_t, "CBasePlayer->m_viewPunchAngle")
	NETVAR(get_aim_punch_angle(), qangle_t, "CBasePlayer->m_aimPunchAngle")
	NETVAR(get_aim_punch_angle_vel(), vec3_t, "CBasePlayer->m_aimPunchAngleVel")
	NETVAR(get_view_model(), c_base_handle, "CBasePlayer->m_hViewModel[0]")

	VFUNC(set_local_view_angles(qangle_t& angle), 372, void(__thiscall*)(void*, qangle_t&), angle)

	VFUNC(think(), 138, void(__thiscall*)(void*))
	VFUNC(pre_think(), 317, void(__thiscall*)(void*))
	VFUNC(post_think(), 318, void(__thiscall*)(void*))

	CUSTOM_VFUNC(using_standard_weapons_in_vehicle(), "client_panorama.dll", "56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41", bool(__thiscall*)(void*))
	CUSTOM_VFUNC(physics_run_think(int index), "client_panorama.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87", bool(__thiscall*)(void*, int), index)
	CUSTOM_VFUNC(select_item(const char* string, int sub_type = 0), "client_panorama.dll", "55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06", bool(__thiscall*)(void*, const char*, int), string, sub_type)

	void reset_anim_state(c_anim_state* state);
	void update_anim_state(c_anim_state* state, qangle_t angle);
	void create_anim_state(c_anim_state* state);

	bool is_alive() { return get_life_state() == LIFE_ALIVE; }

	vec3_t get_hitbox_position(int hitbox, matrix3x4_t* matrix = nullptr);

	vec3_t get_bone_position(int id, matrix3x4_t* matrix) { return vec3_t(matrix[id][0][3], matrix[id][1][3], matrix[id][2][3]); }

	vec3_t get_bone_position(int id) {
		vec3_t position, rotation;
		static const auto get_bone_position_fn = reinterpret_cast<void(__thiscall*)(void*, int, vec3_t*, vec3_t*)>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 30 8D"));
		get_bone_position_fn(this, id, &position, &rotation);
		return position;
	}

	vec3_t get_eye_position() {
		vec3_t out;
		memory::get_vfunc<void(__thiscall*)(void*, vec3_t&)>(this, 284)(this, out);
		out.z -= get_view_offset().z - floor(get_view_offset().z);
		return out;
	}
};

class c_base_animating : public c_base_player {
public:
	PPOFFSET(get_anim_layers(), anim_layers, 0x2990)
	NETVAR(get_pose_params(), pose_params, "CBaseAnimating->m_flPoseParameter")
	NETPROP(get_client_side_animation_prop(), "CBaseAnimating->m_bClientSideAnimation")
	NETVAR(get_client_side_animation(), bool, "CBaseAnimating->m_bClientSideAnimation")
	NETVAR(get_sequence(), int, "CBaseAnimating->m_nSequence")

	void set_anim_layers(anim_layers layers) { std::copy(std::begin(layers), std::end(layers), std::begin(get_anim_layers())); }

	void set_pose_params(pose_params params) { std::copy(std::begin(params), std::end(params), std::begin(get_pose_params())); }

	void set_pose_parameter(int param, float value) {
		static const auto studio_set_pose_parameter_fn = reinterpret_cast<void*>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 08 F3 0F 11 54 24 ? 85 D2"));

		auto result = 0.0f;
		auto hdr = get_model_hdr();

		__asm {
			pushad
			movss xmm2, [value]
			lea eax, [result]
			push eax
			mov edx, param
			mov ecx, hdr
			call studio_set_pose_parameter_fn
			pop eax
			popad
		}

		get_pose_params()[param] = result;
	}

	int get_sequence_activity(int sequence);
};

class c_cs_player : public c_base_animating {
public:
	NETVAR(get_velocity_modifier(), float, "CCSPlayer->m_flVelocityModifier")
	NETVAR(has_defuser(), bool, "CCSPlayer->m_bHasDefuser")
	NETVAR(has_helmet(), bool, "CCSPlayer->m_bHasHelmet")
	NETVAR(get_armor_value(), int, "CCSPlayer->m_ArmorValue")
	NETVAR(has_heavy_armor(), bool, "CCSPlayer->m_bHasHeavyArmor")
	NETVAR(get_eye_angles(), qangle_t, "CCSPlayer->m_angEyeAngles")
	NETVAR(is_scoped(), bool, "CCSPlayer->m_bIsScoped")
	NETVAR(is_immune(), bool, "CCSPlayer->m_bGunGameImmunity")
	NETVAR(get_health_boost_time(), float, "CCSPlayer->m_flHealthShotBoostExpirationTime")
	NETVAR(get_lby(), float, "CCSPlayer->m_flLowerBodyYawTarget")
	NETVAR_OFFSET(get_flash_alpha(), float, "CCSPlayer->m_flFlashMaxAlpha", -0x8)

	OFFSET(get_anim_state(), c_anim_state*, 0x3900)

	VFUNC(standard_blending_rules(studiohdr_t* hdr, vec3_t* vec, quaternion* q, const float time, const int mask), 205, void(__thiscall*)(void*, studiohdr_t*, vec3_t*, quaternion*, float, int), hdr, vec, q, time, mask)
	VFUNC(build_transformations(studiohdr_t* hdr, vec3_t* vec, quaternion* q, matrix3x4_t& transform, const int mask, uint8_t* computed), 189, void(__thiscall*)(void*, studiohdr_t*, vec3_t*, quaternion*, matrix3x4_t const&, int, uint8_t*), hdr, vec, q, transform, mask, computed)
	VFUNC(update_ik_locks(float time), 191, void(__thiscall*)(void*, float), time)
	VFUNC(calculate_ik_locks(float time), 192, void(__thiscall*)(void*, float), time)
	VFUNC(update_client_side_animation(), 223, void(__thiscall*)(void*))

	void invalidate_physics_recursive(int flags) {
		static const auto invalidate_physics_recursive_fn = reinterpret_cast<void(__thiscall*)(void*, int)>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56"));
		invalidate_physics_recursive_fn(this, flags);
	}

	player_info_t get_info();
};

class c_base_combat_weapon : public c_base_attributable_item {
public:
	VFUNC(get_inaccuracy(), 482, float(__thiscall*)(void*))
	VFUNC(get_spread(), 452, float(__thiscall*)(void*))
	VFUNC(update_accuracy(), 483, void(__thiscall*)(void*))

	NETVAR(get_pin_pulled(), bool, "CBaseCSGrenade->m_bPinPulled")
	NETVAR(get_throw_time(), float, "CBaseCSGrenade->m_fThrowTime")

	NETVAR(get_post_pone_fire_ready_time(), float, "CWeaponCSBase->m_flPostponeFireReadyTime")
	NETVAR(get_accuracy_penalty(), float, "CWeaponCSBase->m_fAccuracyPenalty")
	NETVAR(get_recoil_index(), float, "CWeaponCSBase->m_flRecoilIndex")
	NETVAR(get_last_shot_time(), float, "CWeaponCSBase->m_fLastShotTime")
	NETVAR(get_throw_strength(), float, "CWeaponCSBase->m_flThrowStrength")
	NETVAR(get_ammo(), int, "CBaseCombatWeapon->m_iClip1")
	NETVAR(get_zoom_level(), int, "CWeaponCSBaseGun->m_zoomLevel")
	NETVAR(get_next_primary_attack(), float, "CBaseCombatWeapon->m_flNextPrimaryAttack")
	NETVAR(get_next_secondary_attack(), float, "CBaseCombatWeapon->m_flNextSecondaryAttack")
	NETVAR(get_world_model(), c_base_handle, "CBaseCombatWeapon->m_hWeaponWorldModel")

	std::string get_name();

	c_cs_weapon_data* get_cs_weapon_data();
};

class c_local_player {
	friend bool operator==(const c_local_player& lhs, void* rhs) { return *lhs.m_loc == rhs; }
public:
	c_local_player() : m_loc(nullptr) {}

	operator bool() const { return *m_loc != nullptr; }
	operator c_cs_player*() const { return *m_loc; }

	c_cs_player* operator->() const { return *m_loc; }
private:
	c_cs_player** m_loc;
};