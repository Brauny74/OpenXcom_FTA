#pragma once

/*
 * Copyright 2010-2020 OpenXcom Developers.
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
#include <vector>
#include <string>

namespace OpenXcom
{
class Text;
class Bar;
class SoldierInfoStatView;
class UnitStats;

class SoldierInfoView
{
  public:
	SoldierInfoStatView *_statViewTimeUnits, *_statViewManeuvering, *_statViewPhysics, *_statViewStamina, *_statViewMissiles, *_statViewChemistry, *_statViewHealth,
		*_statViewDogfight, *_statViewBiology, *_statViewBravery, *_statViewInsight, *_statViewReactions, *_statViewTracking, *_statViewData, *_statViewFiring,
		*_statViewTactics, *_statViewComputers, *_statViewThrowing, *_statViewBeams, *_statViewMaterials, *_statViewMelee, *_statViewSynaptic, *_statViewPsychology,
		*_statViewStrength, *_statViewGravity, *_statViewDesigning, *_statViewMana, *_statViewPsionics, *_statViewPsiStrength, *_statViewXenolinguistics, *_statViewPsiSkill;
	std::vector<SoldierInfoStatView *> *_baseSoldierStats, *_baseScientistStats, *_basePilotStats;
	std::vector<SoldierInfoStatView *> *_allStats;

	SoldierInfoView(int yPos, int step);

	void setSoldierStatsVisible(bool visible);
	void setPilotStatsVisible(bool visible, bool isBeamOperationsUnlockResearched, bool isCraftSynapseUnlockResearched, bool isGravControlUnlockResearched);
	void setScientistStatsVisible(bool visible, bool isPsiRequirementsResearched, bool isXenologyUnlockResearced);
	void hideAllStats();
	void fillNumbers(UnitStats withArmor, UnitStats *current, UnitStats *initial);
};

}
