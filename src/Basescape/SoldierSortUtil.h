#pragma once
/*
 * Copyright 2010-2019 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../Engine/Game.h"
#include "../Savegame/Soldier.h"
#include "../Savegame/SavedGame.h"
#include "../Mod/Mod.h"

namespace OpenXcom
{

typedef int (*getStatFn_t)(Game *, Soldier *);

struct SortFunctor
{
	typedef Soldier* first_argument_type;
	typedef Soldier* second_argument_type;
	typedef bool result_type;

	Game *_game;
	getStatFn_t _getStatFn;
	SortFunctor(Game *game, getStatFn_t getStatFn) : _game(game), _getStatFn(getStatFn) { }
	bool operator()(Soldier *a, Soldier *b)
	{
		bool ret = _getStatFn(_game, a) < _getStatFn(_game, b);
		return ret;
	}
	getStatFn_t getGetter()
	{
		return _getStatFn;
	}
};

#define GET_ATTRIB_STAT_FN(attrib) \
	int attrib##Stat(Game *game, Soldier *s);
GET_ATTRIB_STAT_FN(tu)
GET_ATTRIB_STAT_FN(stamina)
GET_ATTRIB_STAT_FN(health)
GET_ATTRIB_STAT_FN(bravery)
GET_ATTRIB_STAT_FN(reactions)
GET_ATTRIB_STAT_FN(firing)
GET_ATTRIB_STAT_FN(throwing)
GET_ATTRIB_STAT_FN(strength)
GET_ATTRIB_STAT_FN(mana)
GET_ATTRIB_STAT_FN(psiStrength)
GET_ATTRIB_STAT_FN(psiSkill)
GET_ATTRIB_STAT_FN(melee)
GET_ATTRIB_STAT_FN(maneuvering)
GET_ATTRIB_STAT_FN(missiles)
GET_ATTRIB_STAT_FN(dogfight)
GET_ATTRIB_STAT_FN(tracking)
GET_ATTRIB_STAT_FN(cooperation)
GET_ATTRIB_STAT_FN(beams)
GET_ATTRIB_STAT_FN(synaptic)
GET_ATTRIB_STAT_FN(gravity)
GET_ATTRIB_STAT_FN(physics)
GET_ATTRIB_STAT_FN(chemistry)
GET_ATTRIB_STAT_FN(biology)
GET_ATTRIB_STAT_FN(insight)
GET_ATTRIB_STAT_FN(data)
GET_ATTRIB_STAT_FN(computers)
GET_ATTRIB_STAT_FN(tactics)
GET_ATTRIB_STAT_FN(materials)
GET_ATTRIB_STAT_FN(psychology)
GET_ATTRIB_STAT_FN(designing)
GET_ATTRIB_STAT_FN(alienTech)
GET_ATTRIB_STAT_FN(psionics)
GET_ATTRIB_STAT_FN(xenolinguistics)
GET_ATTRIB_STAT_FN(weaponry)
GET_ATTRIB_STAT_FN(explosives)
GET_ATTRIB_STAT_FN(efficiency)
GET_ATTRIB_STAT_FN(microelectronics)
GET_ATTRIB_STAT_FN(metallurgy)
GET_ATTRIB_STAT_FN(processing)
GET_ATTRIB_STAT_FN(hacking)
GET_ATTRIB_STAT_FN(construction)
GET_ATTRIB_STAT_FN(diligence)
GET_ATTRIB_STAT_FN(reverseEngineering)
GET_ATTRIB_STAT_FN(stealth)
GET_ATTRIB_STAT_FN(perseption)
GET_ATTRIB_STAT_FN(charisma)
GET_ATTRIB_STAT_FN(investigation)
GET_ATTRIB_STAT_FN(deception)
GET_ATTRIB_STAT_FN(interrogation)

#undef GET_ATTRIB_STAT_FN

#define GET_ATTRIB_STAT_FN(attrib) \
	int attrib##StatBase(Game *game, Soldier *s);
GET_ATTRIB_STAT_FN(tu)
GET_ATTRIB_STAT_FN(stamina)
GET_ATTRIB_STAT_FN(health)
GET_ATTRIB_STAT_FN(bravery)
GET_ATTRIB_STAT_FN(reactions)
GET_ATTRIB_STAT_FN(firing)
GET_ATTRIB_STAT_FN(throwing)
GET_ATTRIB_STAT_FN(strength)
GET_ATTRIB_STAT_FN(mana)
GET_ATTRIB_STAT_FN(psiStrength)
GET_ATTRIB_STAT_FN(psiSkill)
GET_ATTRIB_STAT_FN(melee)
GET_ATTRIB_STAT_FN(maneuvering)
GET_ATTRIB_STAT_FN(missiles)
GET_ATTRIB_STAT_FN(dogfight)
GET_ATTRIB_STAT_FN(tracking)
GET_ATTRIB_STAT_FN(cooperation)
GET_ATTRIB_STAT_FN(beams)
GET_ATTRIB_STAT_FN(synaptic)
GET_ATTRIB_STAT_FN(gravity)
GET_ATTRIB_STAT_FN(physics)
GET_ATTRIB_STAT_FN(chemistry)
GET_ATTRIB_STAT_FN(biology)
GET_ATTRIB_STAT_FN(insight)
GET_ATTRIB_STAT_FN(data)
GET_ATTRIB_STAT_FN(computers)
GET_ATTRIB_STAT_FN(tactics)
GET_ATTRIB_STAT_FN(materials)
GET_ATTRIB_STAT_FN(psychology)
GET_ATTRIB_STAT_FN(designing)
GET_ATTRIB_STAT_FN(alienTech)
GET_ATTRIB_STAT_FN(psionics)
GET_ATTRIB_STAT_FN(xenolinguistics)
GET_ATTRIB_STAT_FN(weaponry)
GET_ATTRIB_STAT_FN(explosives)
GET_ATTRIB_STAT_FN(efficiency)
GET_ATTRIB_STAT_FN(microelectronics)
GET_ATTRIB_STAT_FN(metallurgy)
GET_ATTRIB_STAT_FN(processing)
GET_ATTRIB_STAT_FN(hacking)
GET_ATTRIB_STAT_FN(construction)
GET_ATTRIB_STAT_FN(diligence)
GET_ATTRIB_STAT_FN(reverseEngineering)
GET_ATTRIB_STAT_FN(stealth)
GET_ATTRIB_STAT_FN(perseption)
GET_ATTRIB_STAT_FN(charisma)
GET_ATTRIB_STAT_FN(investigation)
GET_ATTRIB_STAT_FN(deception)
GET_ATTRIB_STAT_FN(interrogation)
#undef GET_ATTRIB_STAT_FN

#define GET_ATTRIB_STAT_FN(attrib) \
	int attrib##StatPlus(Game *game, Soldier *s);
GET_ATTRIB_STAT_FN(tu)
GET_ATTRIB_STAT_FN(stamina)
GET_ATTRIB_STAT_FN(health)
GET_ATTRIB_STAT_FN(bravery)
GET_ATTRIB_STAT_FN(reactions)
GET_ATTRIB_STAT_FN(firing)
GET_ATTRIB_STAT_FN(throwing)
GET_ATTRIB_STAT_FN(strength)
GET_ATTRIB_STAT_FN(mana)
GET_ATTRIB_STAT_FN(psiStrength)
GET_ATTRIB_STAT_FN(psiSkill)
GET_ATTRIB_STAT_FN(melee)
GET_ATTRIB_STAT_FN(maneuvering)
GET_ATTRIB_STAT_FN(missiles)
GET_ATTRIB_STAT_FN(dogfight)
GET_ATTRIB_STAT_FN(tracking)
GET_ATTRIB_STAT_FN(cooperation)
GET_ATTRIB_STAT_FN(beams)
GET_ATTRIB_STAT_FN(synaptic)
GET_ATTRIB_STAT_FN(gravity)
GET_ATTRIB_STAT_FN(physics)
GET_ATTRIB_STAT_FN(chemistry)
GET_ATTRIB_STAT_FN(biology)
GET_ATTRIB_STAT_FN(insight)
GET_ATTRIB_STAT_FN(data)
GET_ATTRIB_STAT_FN(computers)
GET_ATTRIB_STAT_FN(tactics)
GET_ATTRIB_STAT_FN(materials)
GET_ATTRIB_STAT_FN(psychology)
GET_ATTRIB_STAT_FN(designing)
GET_ATTRIB_STAT_FN(alienTech)
GET_ATTRIB_STAT_FN(psionics)
GET_ATTRIB_STAT_FN(xenolinguistics)
GET_ATTRIB_STAT_FN(weaponry)
GET_ATTRIB_STAT_FN(explosives)
GET_ATTRIB_STAT_FN(efficiency)
GET_ATTRIB_STAT_FN(microelectronics)
GET_ATTRIB_STAT_FN(metallurgy)
GET_ATTRIB_STAT_FN(processing)
GET_ATTRIB_STAT_FN(hacking)
GET_ATTRIB_STAT_FN(construction)
GET_ATTRIB_STAT_FN(diligence)
GET_ATTRIB_STAT_FN(reverseEngineering)
GET_ATTRIB_STAT_FN(stealth)
GET_ATTRIB_STAT_FN(perseption)
GET_ATTRIB_STAT_FN(charisma)
GET_ATTRIB_STAT_FN(investigation)
GET_ATTRIB_STAT_FN(deception)
GET_ATTRIB_STAT_FN(interrogation)
#undef GET_ATTRIB_STAT_FN

#define GET_SOLDIER_STAT_FN(attrib, camelCaseAttrib) \
	int attrib##Stat(Game *game, Soldier *s);
GET_SOLDIER_STAT_FN(id, Id)
GET_SOLDIER_STAT_FN(name, Name)
GET_SOLDIER_STAT_FN(type, Type)
GET_SOLDIER_STAT_FN(role, Role)
GET_SOLDIER_STAT_FN(roleRank, RoleRank)
GET_SOLDIER_STAT_FN(rank, Rank)
GET_SOLDIER_STAT_FN(missions, Missions)
GET_SOLDIER_STAT_FN(kills, Kills)
GET_SOLDIER_STAT_FN(woundRecovery, WoundRecovery)
GET_SOLDIER_STAT_FN(manaMissing, ManaMissing)
GET_SOLDIER_STAT_FN(idleDays, IdleDays)
#undef GET_SOLDIER_STAT_FN

}
