/*
 * Copyright 2010-2022 OpenXcom Developers.
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
#include "../Engine/Action.h"
#include "../Engine/Game.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Options.h"
#include "../Engine/RNG.h"
#include "../Interface/Text.h"
#include "../Interface/TextButton.h"
#include "../Interface/TextList.h"
#include "../Interface/Window.h"
#include "../Mod/Mod.h"
#include "../Mod/RuleResearch.h"
#include "../Mod/RuleInterface.h"
#include "../Savegame/Base.h"
#include "../Savegame/ItemContainer.h"
#include "../Savegame/ResearchProject.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/Soldier.h"
#include "ResearchInfoStateFtA.h"
#include "../Basescape/ResearchAllocateScientists.h"
#include <climits>

namespace OpenXcom
{

/**
 * Initializes all the elements in the ResearchProject screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 * @param rule A RuleResearch which will be used to create a new ResearchProject
 */
ResearchInfoStateFtA::ResearchInfoStateFtA(Base *base, const RuleResearch *rule) : _base(base), _rule(rule)
{
	_newProject = true;
	buildUi();

}

/**
 * Initializes all the elements in the ResearchProject screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 * @param project A ResearchProject to modify
 */
ResearchInfoStateFtA::ResearchInfoStateFtA(Base *base, ResearchProject *project) : _base(base), _project(project)
{
	_newProject = false;
	//getAssignedScientists();
	buildUi();
}



/**
 * Builds dialog.
 */
void ResearchInfoStateFtA::buildUi()
{
	_screen = false;

	_window = new Window(this, 320, 156, 0, 22);
	_txtTitle = new Text(302, 17, 9, 31);

	_txtAvailableScientist = new Text(166, 9, 9, 52);
	_txtAvailableSpace = new Text(166, 9, 9, 62);
	_txtGrade = new Text(32, 9, 125, 80);
	_txtStat1 = new Text(18, 9, 157, 80);
	_txtStat2 = new Text(18, 9, 157, 80);
	_txtStat3 = new Text(18, 9, 157, 80);
	_txtStat4 = new Text(18, 9, 157, 80);
	_txtStat5 = new Text(18, 9, 157, 80);
	_txtStat6 = new Text(18, 9, 157, 80);
	_txtInsight = new Text(18, 9, 157, 80); //x offset later

	_btnOk = new TextButton(148, 16, 165, 153);
	_btnCancel = new TextButton(148, 16, 9, 153);
	_btnAllocate = new TextButton(110, 16, 9, 72);
	_btnAbandon = new TextButton(115, 16, 188, 56);
	_lstScientists = new TextList(301, 63, 9, 89);

	// Set palette
	setInterface("allocateResearch");

	add(_window, "window", "allocateResearch");
	add(_btnOk, "button2", "allocateResearch");
	add(_btnCancel, "button2", "allocateResearch");
	add(_btnAllocate, "button2", "allocateResearch");
	add(_btnAbandon, "button3", "allocateResearch");
	add(_txtTitle, "text", "allocateResearch");
	add(_txtAvailableScientist, "text", "allocateResearch");
	add(_txtAvailableSpace, "text", "allocateResearch");
	add(_txtGrade, "text", "allocateResearch");
	add(_txtStat1, "text", "allocateResearch");
	add(_txtStat2, "text", "allocateResearch");
	add(_txtStat3, "text", "allocateResearch");
	add(_txtStat4, "text", "allocateResearch");
	add(_txtStat5, "text", "allocateResearch");
	add(_txtStat6, "text", "allocateResearch");
	add(_txtInsight, "text", "allocateResearch");
	add(_lstScientists, "list", "allocateResearch");

	centerAllSurfaces();

	// Set up objects
	setWindowBackground(_window, "allocateResearch");

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr(getResearchRules()->getName()));

	_btnAllocate->setText(tr("STR_ALLOCATE_SCIENTISTS"));
	_btnAllocate->onMouseClick((ActionHandler)&ResearchInfoStateFtA::btnAllocateClick);

	_btnOk->onMouseClick((ActionHandler)&ResearchInfoStateFtA::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&ResearchInfoStateFtA::btnOkClick, Options::keyOk);

	if (_newProject)
	{
		_btnOk->setText(tr("STR_START_PROJECT"));
		//_btnOk->setColor(_game->getMod()->getInterface("allocateResearch")->getElement("button1")->color);
		_btnAbandon->setVisible(false);
	}
	else
	{
		_btnOk->setText(tr("STR_OK"));
	}

	_btnCancel->setText(tr("STR_CANCEL_UC"));
	_btnCancel->onMouseClick((ActionHandler)&ResearchInfoStateFtA::btnCancelClick);
	_btnCancel->onKeyboardPress((ActionHandler)&ResearchInfoStateFtA::btnCancelClick, Options::keyCancel);

	_btnAbandon->setText(tr("STR_ABANDON_PROJECT"));
	_btnAbandon->onMouseClick((ActionHandler)&ResearchInfoStateFtA::btnAbandonClick);

	_txtGrade->setText(tr("GRADE"));

	setAssignedScientists();

	unsigned int x = 157;
	unsigned int offset = 18;
	int stat = getStatString(0).first;
	if (stat > 0)
	{
		_txtStat1->setText(tr(getStatString(0).second));
		_txtStat1->setX(x);
		x += offset;
	}

	stat = getStatString(1).first;
	if (stat > 0)
	{
		_txtStat2->setText(tr(getStatString(1).second));
		_txtStat2->setX(x);
		x += offset;
	}

	stat = getStatString(2).first;
	if (stat > 0)
	{
		_txtStat3->setText(tr(getStatString(2).second));
		_txtStat3->setX(x);
		x += offset;
	}

	stat = getStatString(3).first;
	if (stat > 0)
	{
		_txtStat4->setText(tr(getStatString(3).second));
		_txtStat4->setX(x);
		x += offset;
	}

	stat = getStatString(4).first;
	if (stat > 0)
	{
		_txtStat5->setText(tr(getStatString(4).second));
		_txtStat5->setX(x);
		x += offset;
	}

	stat = getStatString(5).first;
	if (stat > 0)
	{
		_txtStat6->setText(tr(getStatString(5).second));
		_txtStat6->setX(x);
		x += offset;
	}

	_txtInsight->setText(tr("STR_INSIGHT_SHORT"));
	_txtInsight->setX(x);

	_lstScientists->setColumns(9, 116, 32, 18, 18, 18, 18, 18, 18, 18);
	_lstScientists->setAlign(ALIGN_CENTER);
	_lstScientists->setAlign(ALIGN_LEFT, 0);
	//_lstSoldierStats->setDot(true);
	//_lstScientists->setColumns(columns, 158, 58, 70);
	_lstScientists->setBackground(_window);
	//_lstScientists->setMargin(2);
	_lstScientists->setWordWrap(true);

	if (_newProject)
	{
		// mark new as normal
		if (_game->getSavedGame()->isResearchRuleStatusNew(getResearchRules()->getName()))
		{
			_game->getSavedGame()->setResearchRuleStatus(getResearchRules()->getName(), RuleResearch::RESEARCH_STATUS_NORMAL);
		}
	}
}

/**
 * Frees up memory that's not automatically cleaned on exit
 */
ResearchInfoStateFtA::~ResearchInfoStateFtA()
{
}

/**
 * Updates the research list
 * after going to other screens.
 */
void ResearchInfoStateFtA::init()
{
	State::init();
	fillScientistsList(0);
}


const RuleResearch* ResearchInfoStateFtA::getResearchRules()
{
	if (_newProject)
	{
		return _rule;
	}
	else
	{
		return _project->getRules();
	}
}

int ResearchInfoStateFtA::GetStatValue(Soldier &s, const std::string &desc)
{
	UnitStats *sStats = s.getCurrentStats();
	if (desc == "STR_PHYSICS_SHORT")
		return sStats->physics;
	if (desc == "STR_CHEMISTRY_SHORT")
		return sStats->chemistry;
	if (desc == "STR_BIOLOGY_SHORT")
		return sStats->biology;
	if (desc == "STR_DATA_ANALISYS_SHORT")
		return sStats->data;
	if (desc == "STR_COMPUTER_SCIENCE_SHORT")
		return sStats->computers;
	if (desc == "STR_MATERIAL_SCIENCE_SHORT")
		return sStats->materials;
	/*if (desc == "STR_PSYCHOLOGY_SHORT")
		return sStats->psychology;*/
	if (desc == "STR_DESIGNING_SHORT")
		return sStats->designing;
	if (desc == "STR_PSIONICS_SHORT")
		return sStats->psionics;
	if (desc == "STR_XENOLINGUISTICS_SHORT")
		return sStats->xenolinguistics;
	return 0;
}

void ResearchInfoStateFtA::fillScientistsList(size_t scrl)
{
	_lstScientists->clearList();
	std::vector<std::string> stats(7, "");
	for (auto s : _scientists)
	{
		std::ostringstream ss;
		ss << s->getRoleRank(ROLE_SCIENTIST);
		size_t i = 0;
		for (auto stat : ResearchStats)
		{
			stats[i++] = std::to_string(GetStatValue(*s, stat.second));
		}
		stats[i] = std::to_string(s->getCurrentStats()->insight);
		_lstScientists->addRow(9, s->getName().c_str(), ss.str().c_str(), stats[0].c_str(), stats[1].c_str(), stats[2].c_str(),
							   stats[3].c_str(), stats[4].c_str(), stats[5].c_str(), stats[6].c_str());
	}
}


/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void ResearchInfoStateFtA::btnOkClick(Action *)
{
	if (_newProject)
	{
		int rng = RNG::generate(50, 150);
		int randomizedCost = getResearchRules()->getCost() * rng / 100;
		if (getResearchRules()->getCost() > 0)
		{
			randomizedCost = std::max(1, randomizedCost);
		}
		_project = new ResearchProject(getResearchRules(), randomizedCost);
		_base->addResearch(_project);
		if (getResearchRules()->needItem() && getResearchRules()->destroyItem())
		{
			_base->getStorageItems()->removeItem(getResearchRules()->getName(), 1);
		}
	}

	for (auto s : _scientists)
	{
		s->setResearchProject(_project);
		s->setPsiTraining(false);
		s->setTraining(false);
		s->setCraft(0);
		s->setReturnToTrainingWhenHealed(false); //just in case
	}

	_game->popState();
}

/**
 * Returns to the previous screen, removing the current project from the active
 * research list.
 * @param action Pointer to an action.
 */
void ResearchInfoStateFtA::btnCancelClick(Action *)
{
	_game->popState();
}

/**
 * Returns to the previous screen, removing the current project from the active
 * research list.
 * @param action Pointer to an action.
 */
void ResearchInfoStateFtA::btnAbandonClick(Action *)
{
	for (auto s : _scientists)
	{
		s->setResearchProject(0);
	}
	_base->removeResearch(_project);
	_game->popState();
}

/**
 * Opens Allocate Scientists state.
 * @param action Pointer to an action.
 */
void ResearchInfoStateFtA::btnAllocateClick(Action *action)
{
	_game->pushState(new ResearchAllocateScientists(_base, this));
}

/**
 * Updates count of assigned/free scientists and available lab space.
 */
void ResearchInfoStateFtA::setAssignedScientists()
{
	size_t freeScientists = 0;
	auto recovery = _base->getSumRecoveryPerDay();
	bool isBusy = false, isFree = false;

	for (auto s : _base->getPersonnel(ROLE_SCIENTIST))
	{
		s->getCurrentDuty(_game->getLanguage(), recovery, isBusy, isFree);
		if (s->getResearchProject() && s->getResearchProject()->getRules() == this->getResearchRules())
		{
			_scientists.insert(s);
		}
		else if (!isBusy && isFree)
		{
			if (_scientists.find(s) != _scientists.end())
			{ }
			else
				freeScientists++;
		}
	}
	_txtAvailableScientist->setText(tr("STR_SCIENTISTS_AVAILABLE_UC").arg(freeScientists));

	size_t teamSize = _scientists.size();
	for (auto s : _scientists)
	{
		if (s->getResearchProject() && s->getResearchProject()->getRules() != this->getResearchRules())
		{
			teamSize--;
		}
	}
	_workSpace = _base->getFreeLaboratories(true, _project) - teamSize;
	_txtAvailableSpace->setText(tr("STR_LABORATORY_SPACE_AVAILABLE_UC").arg(_workSpace));
}

std::pair<int, std::string> ResearchInfoStateFtA::getStatString(size_t position)
{
	auto stats = getResearchRules()->getStats();
	std::map<int, std::string> statMap;

	if (stats.physics > 0)
	{
		statMap.insert(std::make_pair(stats.physics, tr("STR_PHYSICS_SHORT")));
		ResearchStats.insert(std::make_pair(stats.physics, "STR_PHYSICS_SHORT"));
	}
	if (stats.chemistry > 0)
	{
		statMap.insert(std::make_pair(stats.chemistry, tr("STR_CHEMISTRY_SHORT")));
		ResearchStats.insert(std::make_pair(stats.chemistry, "STR_CHEMISTRY_SHORT"));
	}
	if (stats.biology > 0)
	{
		statMap.insert(std::make_pair(stats.biology, tr("STR_BIOLOGY_SHORT")));
		ResearchStats.insert(std::make_pair(stats.biology, "STR_BIOLOGY_SHORT"));
	}
	if (stats.data > 0)
	{
		statMap.insert(std::make_pair(stats.data, tr("STR_DATA_ANALISIS_SHORT")));
		ResearchStats.insert(std::make_pair(stats.data, "STR_DATA_ANALISIS_SHORT"));
	}
	if (stats.computers > 0)
	{
		statMap.insert(std::make_pair(stats.computers, tr("STR_COMPUTER_SCIENCE_SHORT")));
		ResearchStats.insert(std::make_pair(stats.computers, "STR_COMPUTER_SCIENCE_SHORT"));
	}
	if (stats.tactics > 0)
	{
		statMap.insert(std::make_pair(stats.tactics, tr("STR_TACTICS_SHORT")));
		ResearchStats.insert(std::make_pair(stats.tactics, "STR_TACTICS_SHORT"));
	}
	if (stats.materials > 0)
	{
		statMap.insert(std::make_pair(stats.materials, tr("STR_MATERIAL_SCIENCE_SHORT")));
		ResearchStats.insert(std::make_pair(stats.materials, "STR_MATERIAL_SCIENCE_SHORT"));
	}
	if (stats.designing > 0)
	{
		statMap.insert(std::make_pair(stats.designing, tr("STR_DESIGNING_SHORT")));
		ResearchStats.insert(std::make_pair(stats.designing, "STR_DESIGNING_SHORT"));
	}
	if (stats.psionics > 0)
	{
		statMap.insert(std::make_pair(stats.psionics, tr("STR_PSIONICS_SHORT")));
		ResearchStats.insert(std::make_pair(stats.psionics, "STR_PSIONICS_SHORT"));
	}
	if (stats.xenolinguistics > 0)
	{
		statMap.insert(std::make_pair(stats.xenolinguistics, tr("STR_XENOLINGUISTICS_SHORT")));
		ResearchStats.insert(std::make_pair(stats.xenolinguistics, "STR_XENOLINGUISTICS_SHORT"));
	}

	size_t pos = 0;
	std::pair<int, std::string> result;
	for (auto it = statMap.rbegin(); it != statMap.rend(); ++it)
	{
		if (pos == position)
		{
			result = std::make_pair((*it).first, (*it).second);
			break;
		}
		pos++;
	}

	return result;
}

}
