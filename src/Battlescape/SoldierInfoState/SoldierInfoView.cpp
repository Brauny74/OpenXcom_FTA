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

#include "SoldierInfoView.h"
#include <vector>
#include "../../Interface/Bar.h"
#include "../../Interface/TextButton.h"
#include "../../Interface/Text.h"
#include "SoldierInfoStatView.h"
#include "../../Mod/Unit.h"
namespace OpenXcom
{

OpenXcom::SoldierInfoView::SoldierInfoView(int yPos, int step)
{
	_statViewTimeUnits = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewManeuvering = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewPhysics = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewStamina = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewMissiles = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewChemistry = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewHealth = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewDogfight = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewBiology = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewBravery = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewInsight = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewReactions = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewTracking = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewData = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewFiring = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewTactics = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewComputers = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewThrowing = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewBeams = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewMaterials = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewMelee = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewSynaptic = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewPsychology = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewStrength = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewGravity = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewDesigning = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewMana = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewPsionics = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewPsiStrength = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	_statViewXenolinguistics = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));
	yPos = yPos + step;
	_statViewPsiSkill = new SoldierInfoStatView(new Text(120, 9, 6, yPos), new Text(18, 9, 131, yPos), new Bar(170, 7, 150, yPos));

	_baseSoldierStats = new std::vector<SoldierInfoStatView *>{
		_statViewTimeUnits,
		_statViewStamina,
		_statViewHealth,
		_statViewBravery,
		_statViewReactions,
		_statViewFiring,
		_statViewThrowing,
		_statViewMelee,
		_statViewStrength
	};
	_basePilotStats = new std::vector<SoldierInfoStatView *>{
		_statViewManeuvering,
		_statViewMissiles,
		_statViewDogfight,
		_statViewBravery,
		_statViewTracking,
		_statViewTactics
	};
	_baseScientistStats = new std::vector<SoldierInfoStatView *>{
		_statViewPhysics,
		_statViewChemistry,
		_statViewBiology,
		_statViewInsight,
		_statViewData,
		_statViewComputers,
		_statViewMaterials,
		_statViewPsychology,
		_statViewDesigning
	};
	_allStats = new std::vector<SoldierInfoStatView *>{
		_statViewTimeUnits, _statViewManeuvering, _statViewPhysics, _statViewStamina, _statViewMissiles, _statViewChemistry, _statViewHealth,
		_statViewDogfight, _statViewBiology, _statViewBravery, _statViewInsight, _statViewReactions, _statViewTracking, _statViewData, _statViewFiring,
		_statViewTactics, _statViewComputers, _statViewThrowing, _statViewBeams, _statViewMaterials, _statViewMelee, _statViewSynaptic, _statViewPsychology,
		_statViewStrength, _statViewGravity, _statViewDesigning, _statViewMana, _statViewPsionics, _statViewPsiStrength, _statViewXenolinguistics, _statViewPsiSkill
	};
}


void SoldierInfoView::setSoldierStatsVisible(bool visible)
{
	for (std::vector<SoldierInfoStatView *>::iterator stat = _baseSoldierStats->begin(); stat != _baseSoldierStats->end(); stat++)
	{
		(*stat)->setVisible(visible);
	}
}

/// Set visible "true" only with parametrs - isBeamOperationsUnlockResearched, isCraftSynapseUnlockResearched, isGravControlUnlockResearched
/// Set visible "false" can without parametrs
void SoldierInfoView::setPilotStatsVisible(bool visible, bool isBeamOperationsUnlockResearched = true, bool isCraftSynapseUnlockResearched = true, bool isGravControlUnlockResearched = true)
{
	for (std::vector<SoldierInfoStatView *>::iterator stat = _basePilotStats->begin(); stat != _basePilotStats->end(); stat++)
	{
		(*stat)->setVisible(visible);
	}
	if (isBeamOperationsUnlockResearched)
	{
		_statViewBeams->setVisible(visible);
	}

	if (isCraftSynapseUnlockResearched)
	{
		_statViewSynaptic->setVisible(visible);
	}

	if (isGravControlUnlockResearched)
	{
		_statViewGravity->setVisible(visible);
	}
}

/// Set visible "true" only with parametrs - isBeamOperationsUnlockResearched, isCraftSynapseUnlockResearched, isGravControlUnlockResearched
/// Set visible "false" can without parametrs
void SoldierInfoView::setScientistStatsVisible(bool visible, bool isPsiRequirementsResearched = true, bool isXenologyUnlockResearced = true)
{
	for (std::vector<SoldierInfoStatView *>::iterator stat = _baseScientistStats->begin(); stat != _baseScientistStats->end(); stat++)
	{
		(*stat)->setVisible(visible);
	}

	if (isPsiRequirementsResearched)
	{
		_statViewPsionics->setVisible(visible);
	}

	if (isXenologyUnlockResearced)
	{
		_statViewXenolinguistics->setVisible(visible);
	}
}

void OpenXcom::SoldierInfoView::hideAllStats()
{
	for (std::vector<SoldierInfoStatView *>::iterator stat = _allStats->begin(); stat != _allStats->end(); stat++)
	{
		(*stat)->setVisible(false);
	}
}

void SoldierInfoView::fillNumbers(UnitStats withArmor, UnitStats *current, UnitStats *initial)
{
	std::ostringstream ss1;
	ss1 << withArmor.tu;
	_statViewTimeUnits->setValue(ss1.str(), current->tu, withArmor.tu, std::min(withArmor.tu, initial->tu));
	std::ostringstream ss21;
	ss21 << withArmor.maneuvering;
	_statViewManeuvering->setValue(ss21.str(), current->maneuvering, withArmor.maneuvering, std::min(withArmor.maneuvering, initial->maneuvering));
	std::ostringstream ss31;
	ss31 << withArmor.physics;
	_statViewPsionics->setValue(ss31.str(), current->physics, withArmor.physics, std::min(withArmor.physics, initial->physics));

	std::ostringstream ss2;
	ss2 << withArmor.stamina;
	_statViewStamina->setValue(ss2.str(), current->stamina, withArmor.stamina, std::min(withArmor.stamina, initial->stamina));
	std::ostringstream ss22;
	ss22 << withArmor.missiles;
	_statViewMissiles->setValue(ss22.str(), current->missiles, withArmor.missiles, std::min(withArmor.missiles, initial->missiles));
	std::ostringstream ss32;
	ss32 << withArmor.chemistry;
	_statViewChemistry->setValue(ss32.str(), current->chemistry, withArmor.chemistry, std::min(withArmor.chemistry, initial->chemistry));

	std::ostringstream ss3;
	ss3 << withArmor.health;
	_statViewHealth->setValue(ss3.str(), current->health, withArmor.health, std::min(withArmor.health, initial->health));
	std::ostringstream ss23;
	ss23 << withArmor.dogfight;
	_statViewDogfight->setValue(ss23.str(), current->dogfight, withArmor.dogfight, std::min(withArmor.dogfight, initial->dogfight));
	std::ostringstream ss33;
	ss33 << withArmor.biology;
	_statViewBiology->setValue(ss33.str(), current->biology, withArmor.biology, std::min(withArmor.biology, initial->biology));

	std::ostringstream ss4;
	ss4 << withArmor.bravery;
	_statViewBravery->setValue(ss4.str(), current->bravery, withArmor.bravery, std::min(withArmor.bravery, initial->bravery));
	std::ostringstream ss34;
	ss34 << withArmor.insight;
	_statViewInsight->setValue(ss34.str(), current->insight, withArmor.insight, std::min(withArmor.insight, initial->insight));

	std::ostringstream ss5;
	ss5 << withArmor.reactions;
	_statViewReactions->setValue(ss5.str(), current->reactions, withArmor.reactions, std::min(withArmor.reactions, initial->reactions));
	std::ostringstream ss25;
	ss25 << withArmor.tracking;
	_statViewTracking->setValue(ss25.str(), current->tracking, withArmor.tracking, std::min(withArmor.tracking, initial->tracking));
	std::ostringstream ss35;
	ss35 << withArmor.data;
	_statViewData->setValue(ss35.str(), current->data, withArmor.data, std::min(withArmor.data, initial->data));

	std::ostringstream ss6;
	ss6 << withArmor.firing;
	_statViewFiring->setValue(ss6.str(), current->firing, withArmor.firing, std::min(withArmor.firing, initial->firing));
	std::ostringstream ss26;
	ss26 << withArmor.tactics;
	_statViewTactics->setValue(ss26.str(), current->tactics, withArmor.tactics, std::min(withArmor.tactics, initial->tactics));
	std::ostringstream ss36;
	ss36 << withArmor.computers;
	_statViewComputers->setValue(ss36.str(), current->computers, withArmor.computers, std::min(withArmor.computers, initial->computers));

	std::ostringstream ss7;
	ss7 << withArmor.throwing;
	_statViewThrowing->setValue(ss7.str(), current->throwing, withArmor.throwing, std::min(withArmor.throwing, initial->throwing));
	std::ostringstream ss27;
	ss27 << withArmor.beams;
	_statViewBeams->setValue(ss27.str(), current->beams, withArmor.beams, std::min(withArmor.beams, initial->beams));
	std::ostringstream ss37;
	ss37 << withArmor.materials;
	_statViewMaterials->setValue(ss37.str(), current->materials, withArmor.materials, std::min(withArmor.materials, initial->materials));

	std::ostringstream ss8;
	ss8 << withArmor.melee;
	_statViewMelee->setValue(ss8.str(), current->melee, withArmor.melee, std::min(withArmor.melee, initial->melee));
	std::ostringstream ss28;
	ss28 << withArmor.synaptic;
	_statViewSynaptic->setValue(ss28.str(), current->synaptic, withArmor.synaptic, std::min(withArmor.synaptic, initial->synaptic));
	std::ostringstream ss38;
	ss38 << withArmor.psychology;
	_statViewPsychology->setValue(ss38.str(), current->psychology, withArmor.psychology, std::min(withArmor.psychology, initial->psychology));

	std::ostringstream ss9;
	ss9 << withArmor.strength;
	_statViewStrength->setValue(ss9.str(), current->strength, withArmor.strength, std::min(withArmor.strength, initial->strength));
	std::ostringstream ss29;
	ss29 << withArmor.gravity;
	_statViewGravity->setValue(ss29.str(), current->gravity, withArmor.gravity, std::min(withArmor.gravity, initial->gravity));
	std::ostringstream ss39;
	ss39 << withArmor.designing;
	_statViewDesigning->setValue(ss39.str(), current->designing, withArmor.designing, std::min(withArmor.designing, initial->designing));

	std::ostringstream ss10;
	ss10 << withArmor.mana;
	_statViewMana->setValue(ss10.str(), current->mana, withArmor.mana, std::min(withArmor.mana, initial->mana));
	std::ostringstream ss310;
	ss310 << withArmor.psionics;
	_statViewPsionics->setValue(ss310.str(), current->psionics, withArmor.psionics, std::min(withArmor.psionics, initial->psionics));

	std::ostringstream ss12;
	ss12 << withArmor.psiStrength;
	_statViewPsiStrength->setValue(ss12.str(), current->psiStrength, withArmor.psiStrength, std::min(withArmor.psiStrength, initial->psiStrength));
	std::ostringstream ss312;
	ss312 << withArmor.xenolinguistics;
	_statViewXenolinguistics->setValue(ss312.str(), current->xenolinguistics, withArmor.xenolinguistics, std::min(withArmor.xenolinguistics, initial->xenolinguistics));

	std::ostringstream ss13;
	ss13 << withArmor.psiSkill;
	_statViewPsiSkill->setValue(ss13.str(), current->psiSkill, withArmor.psiSkill, std::min(withArmor.psiSkill, initial->psiSkill));
}

}
