/*
 * Copyright 2010-2016 OpenXcom Developers.
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
#include "NewBattleState.h"
#include <cmath>
#include <algorithm>
#include <yaml-cpp/yaml.h>
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Mod/RuleItem.h"
#include "../Engine/LocalizedText.h"
#include "../Interface/TextButton.h"
#include "../Interface/TextEdit.h"
#include "../Interface/TextList.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/ComboBox.h"
#include "../Interface/Slider.h"
#include "../Interface/Frame.h"
#include "../Savegame/SavedBattleGame.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/Base.h"
#include "../Savegame/Craft.h"
#include "../Savegame/ItemContainer.h"
#include "../Savegame/Soldier.h"
#include "../Battlescape/BattlescapeGenerator.h"
#include "../Battlescape/BriefingState.h"
#include "../Savegame/Ufo.h"
#include "../Savegame/MissionSite.h"
#include "../Savegame/AlienBase.h"
#include "../Mod/RuleCraft.h"
#include "../Mod/RuleTerrain.h"
#include "../Mod/AlienDeployment.h"
#include "../Engine/RNG.h"
#include "../Engine/Action.h"
#include "../Engine/Options.h"
#include "../Engine/Logger.h"
#include "../Basescape/CraftInfoState.h"
#include "../Engine/CrossPlatform.h"
#include "../Mod/RuleAlienMission.h"
#include "../Mod/AlienRace.h"
#include "../Mod/RuleGlobe.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the New Battle window.
 * @param game Pointer to the core game.
 */
NewBattleState::NewBattleState() : _craft(0), _selectType(NewBattleSelectType::MISSION), _isRightClick(false)
{
	// Create objects
	_window = new Window(this, 320, 200, 0, 0, POPUP_BOTH);
	_btnQuickSearch = new TextEdit(this, 48, 9, 264, 183);
	_txtTitle = new Text(320, 17, 0, 9);

	_txtMapOptions = new Text(148, 9, 8, 68);
	_frameLeft = new Frame(148, 96, 8, 78);
	_txtAlienOptions = new Text(148, 9, 164, 68);
	_frameRight = new Frame(148, 96, 164, 78);

	_txtMission = new Text(100, 9, 8, 30);
	_cbxMission = new ComboBox(this, 214, 16, 98, 26);
	_btnMission = new TextButton(16, 16, 81, 26);

	_txtCraft = new Text(100, 9, 8, 50);
	_cbxCraft = new ComboBox(this, 106, 16, 98, 46);
	_btnEquip = new TextButton(106, 16, 206, 46);

	_txtDarkness = new Text(120, 9, 22, 83);
	_slrDarkness = new Slider(120, 16, 22, 93);

	_txtTerrain = new Text(120, 9, 22, 113);
	_cbxTerrain = new ComboBox(this, 120, 16, 22+9, 123);
	_btnTerrain = new TextButton(16, 16, 5+9, 123);

	_txtDepth = new Text(120, 9, 22, 143);
	_slrDepth = new Slider(120, 16, 22, 153);

	_txtDifficulty = new Text(120, 9, 178, 83);
	_cbxDifficulty = new ComboBox(this, 120, 16, 178, 93);

	_txtAlienRace = new Text(120, 9, 178, 113);
	_cbxAlienRace = new ComboBox(this, 120, 16, 178+9, 123);
	_btnAlienRace = new TextButton(16, 16, 161+9, 123);

	_txtAlienTech = new Text(120, 9, 178, 143);
	_slrAlienTech = new Slider(120, 16, 178, 153);

	_btnOk = new TextButton(100, 16, 8, 176);
	_btnCancel = new TextButton(100, 16, 110, 176);
	_btnRandom = new TextButton(100, 16, 212, 176);

	_lstSelect = new TextList(288, 144, 8, 28);

	// Set palette
	setInterface("newBattleMenu");

	add(_window, "window", "newBattleMenu");
	add(_btnQuickSearch, "button1", "newBattleMenu");
	add(_txtTitle, "heading", "newBattleMenu");
	add(_txtMapOptions, "heading", "newBattleMenu");
	add(_frameLeft, "frames", "newBattleMenu");
	add(_txtAlienOptions, "heading", "newBattleMenu");
	add(_frameRight, "frames", "newBattleMenu");

	add(_txtMission, "text", "newBattleMenu");
	add(_txtCraft, "text", "newBattleMenu");
	add(_btnEquip, "button1", "newBattleMenu");

	add(_txtDarkness, "text", "newBattleMenu");
	add(_slrDarkness, "button1", "newBattleMenu");
	add(_txtDepth, "text", "newBattleMenu");
	add(_slrDepth, "button1", "newBattleMenu");
	add(_txtTerrain, "text", "newBattleMenu");
	add(_txtDifficulty, "text", "newBattleMenu");
	add(_txtAlienRace, "text", "newBattleMenu");
	add(_txtAlienTech, "text", "newBattleMenu");
	add(_slrAlienTech, "button1", "newBattleMenu");

	add(_btnOk, "button2", "newBattleMenu");
	add(_btnCancel, "button2", "newBattleMenu");
	add(_btnRandom, "button2", "newBattleMenu");

	add(_btnMission, "button1", "newBattleMenu");
	add(_btnTerrain, "button1", "newBattleMenu");
	add(_btnAlienRace, "button1", "newBattleMenu");

	add(_lstSelect, "list", "newBattleMenu");

	add(_cbxTerrain, "button1", "newBattleMenu");
	add(_cbxAlienRace, "button1", "newBattleMenu");
	add(_cbxDifficulty, "button1", "newBattleMenu");
	add(_cbxCraft, "button1", "newBattleMenu");
	add(_cbxMission, "button1", "newBattleMenu");

	centerAllSurfaces();

	// Set up objects
	setWindowBackground(_window, "newBattleMenu");

	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setBig();
	_txtTitle->setText(tr("STR_MISSION_GENERATOR"));

	_txtMapOptions->setText(tr("STR_MAP_OPTIONS"));

	_frameLeft->setThickness(3);

	_txtAlienOptions->setText(tr("STR_ALIEN_OPTIONS"));

	_frameRight->setThickness(3);

	_txtMission->setText(tr("STR_MISSION"));

	_txtCraft->setText(tr("STR_CRAFT"));

	_txtDarkness->setText(tr("STR_MAP_DARKNESS"));

	_txtDepth->setText(tr("STR_MAP_DEPTH"));

	_txtTerrain->setText(tr("STR_MAP_TERRAIN"));

	_txtDifficulty->setText(tr("STR_DIFFICULTY"));

	_txtAlienRace->setText(tr("STR_ALIEN_RACE"));

	_txtAlienTech->setText(tr("STR_ALIEN_TECH_LEVEL"));

	if (Options::debug)
	{
		_missionTypes = _game->getMod()->getDeploymentsList();
	}
	else
	{
		_missionTypes.reserve(_game->getMod()->getDeploymentsList().size());
		for (auto &deploymentName : _game->getMod()->getDeploymentsList())
		{
			auto depl = _game->getMod()->getDeployment(deploymentName);
			if (depl && !depl->isHidden())
			{
				_missionTypes.push_back(deploymentName);
			}
		}
	}
	// ignore, hardcoded
	{
		auto itr = std::find(_missionTypes.begin(), _missionTypes.end(), RuleCraft::DEFAULT_CRAFT_DEPLOYMENT_PREVIEW);
		if (itr != _missionTypes.end())
		{
			_missionTypes.erase(itr);
		}
	}
	_cbxMission->setOptions(_missionTypes, true);
	_cbxMission->onChange((ActionHandler)&NewBattleState::cbxMissionChange);

	const std::vector<std::string> &crafts = _game->getMod()->getCraftsList();
	for (std::vector<std::string>::const_iterator i = crafts.begin(); i != crafts.end(); ++i)
	{
		RuleCraft *rule = _game->getMod()->getCraft(*i);
		if (rule->getMaxUnits() > 0 && rule->getAllowLanding())
		{
			_crafts.push_back(*i);
		}
	}
	_cbxCraft->setOptions(_crafts, true);
	_cbxCraft->onChange((ActionHandler)&NewBattleState::cbxCraftChange);

	_slrDarkness->setRange(0, 15);

	_slrDepth->setRange(1, 3);

	_cbxTerrain->onChange((ActionHandler)&NewBattleState::cbxTerrainChange);

	std::vector<std::string> difficulty;
	difficulty.push_back(tr("STR_1_BEGINNER"));
	difficulty.push_back(tr("STR_2_EXPERIENCED"));
	difficulty.push_back(tr("STR_3_VETERAN"));
	difficulty.push_back(tr("STR_4_GENIUS"));
	difficulty.push_back(tr("STR_5_SUPERHUMAN"));
	_cbxDifficulty->setOptions(difficulty);

	_slrAlienTech->setRange(0, _game->getMod()->getAlienItemLevels().size()-1);
	if (_game->getMod()->getAlienItemLevels().size() <= 1)
	{
		_slrAlienTech->setVisible(false);
		_txtAlienTech->setVisible(false);
	}

	_btnEquip->setText(tr("STR_EQUIP_CRAFT"));
	_btnEquip->onMouseClick((ActionHandler)&NewBattleState::btnEquipClick);

	_btnRandom->setText(tr("STR_RANDOMIZE"));
	_btnRandom->onMouseClick((ActionHandler)&NewBattleState::btnRandomClick);

	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&NewBattleState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&NewBattleState::btnOkClick, Options::keyOk);

	_btnCancel->setText(tr("STR_CANCEL"));
	_btnCancel->onMouseClick((ActionHandler)&NewBattleState::btnCancelClick);
	_btnCancel->onKeyboardPress((ActionHandler)&NewBattleState::btnCancelClick, Options::keyCancel);

	load();

	// -------------------------------

	bool showExtraButtons = _missionTypes.size() > TFTD_DEPLOYMENTS;
	if (!showExtraButtons)
	{
		_cbxTerrain->setX(_txtTerrain->getX());
		_cbxAlienRace->setX(_txtAlienRace->getX());
	}

	_btnMission->setText("...");
	_btnMission->onMouseClick((ActionHandler)&NewBattleState::btnMissionChange);
	_btnMission->onMouseClick((ActionHandler)&NewBattleState::btnMissionChange, SDL_BUTTON_RIGHT);
	_btnMission->setVisible(showExtraButtons);

	_btnTerrain->setText("...");
	_btnTerrain->onMouseClick((ActionHandler)&NewBattleState::btnTerrainChange);
	_btnTerrain->onMouseClick((ActionHandler)&NewBattleState::btnTerrainChange, SDL_BUTTON_RIGHT);
	_btnTerrain->setVisible(showExtraButtons);

	_btnAlienRace->setText("...");
	_btnAlienRace->onMouseClick((ActionHandler)&NewBattleState::btnAlienRaceChange);
	_btnAlienRace->onMouseClick((ActionHandler)&NewBattleState::btnAlienRaceChange, SDL_BUTTON_RIGHT);
	_btnAlienRace->setVisible(showExtraButtons);

	_lstSelect->setColumns(1, 280);
	_lstSelect->setBackground(_window);
	_lstSelect->setAlign(ALIGN_CENTER);
	_lstSelect->setMargin(8);
	_lstSelect->setSelectable(true);
	_lstSelect->onMouseClick((ActionHandler)&NewBattleState::lstSelectClick);
	_lstSelect->onMouseClick((ActionHandler)&NewBattleState::lstSelectClick, SDL_BUTTON_RIGHT);
	_lstSelect->onMouseClick((ActionHandler)&NewBattleState::lstSelectClick, SDL_BUTTON_MIDDLE);
	_lstSelect->setVisible(false);

	_btnQuickSearch->setText(""); // redraw
	_btnQuickSearch->onEnter((ActionHandler)&NewBattleState::btnQuickSearchApply);
	_btnQuickSearch->setVisible(false);

	_btnCancel->onKeyboardRelease((ActionHandler)&NewBattleState::btnQuickSearchToggle, Options::keyToggleQuickSearch);
}

/**
 *
 */
NewBattleState::~NewBattleState()
{

}

/**
 * Resets the menu music and savegame
 * when coming back from the battlescape.
 */
void NewBattleState::init()
{
	State::init();

	if (_craft == 0)
	{
		load();
	}
}

/**
 * Loads new battle data from a YAML file.
 * @param filename YAML filename.
 */
void NewBattleState::load(const std::string &filename)
{
	std::string s = Options::getMasterUserFolder() + filename + ".cfg";
	if (!CrossPlatform::fileExists(s))
	{
		initSave();
	}
	else
	{
		try
		{
			YAML::Node doc = YAML::Load(*CrossPlatform::readFile(s));
			_cbxMission->setSelected(std::min(doc["mission"].as<size_t>(0), _missionTypes.size() - 1));
			cbxMissionChange(0);
			_cbxCraft->setSelected(std::min(doc["craft"].as<size_t>(0), _crafts.size() - 1));
			_slrDarkness->setValue(doc["darkness"].as<size_t>(0));
			_cbxTerrain->setSelected(std::min(doc["terrain"].as<size_t>(0), _terrainTypes.size() - 1));
			cbxTerrainChange(0);
			_cbxAlienRace->setSelected(std::min(doc["alienRace"].as<size_t>(0), _alienRaces.size() - 1));
			_cbxDifficulty->setSelected(doc["difficulty"].as<size_t>(0));
			_slrAlienTech->setValue(doc["alienTech"].as<size_t>(0));

			if (doc["base"])
			{
				const Mod *mod = _game->getMod();
				SavedGame *save = new SavedGame();

				Base *base = new Base(mod);
				base->load(doc["base"], save, false);
				save->getBases()->push_back(base);

				// Add research
				for (auto& pair : mod->getResearchMap())
				{
					save->addFinishedResearchSimple(pair.second);
				}

				// Generate items
				base->getStorageItems()->getContents()->clear();
				const std::vector<std::string> &items = mod->getItemsList();
				for (std::vector<std::string>::const_iterator i = items.begin(); i != items.end(); ++i)
				{
					RuleItem *rule = _game->getMod()->getItem(*i);
					if (rule->getBattleType() != BT_CORPSE && rule->isRecoverable())
					{
						base->getStorageItems()->addItem(*i, 1);
					}
				}

				// Fix invalid contents
				if (base->getCrafts()->empty())
				{
					std::string craftType = _crafts[_cbxCraft->getSelected()];
					_craft = new Craft(_game->getMod()->getCraft(craftType), base, save->getId(craftType));
					base->getCrafts()->push_back(_craft);
				}
				else
				{
					_craft = base->getCrafts()->front();
					for (std::map<std::string, int>::iterator i = _craft->getItems()->getContents()->begin(); i != _craft->getItems()->getContents()->end(); ++i)
					{
						RuleItem *rule = _game->getMod()->getItem(i->first);
						if (!rule)
						{
							i->second = 0;
						}
					}
				}

				_game->setSavedGame(save);
				save->setGamePtr(_game);
			}
			else
			{
				initSave();
			}
		}
		catch (YAML::Exception &e)
		{
			Log(LOG_WARNING) << e.what();
			initSave();
		}
	}
}

/**
 * Saves new battle data to a YAML file.
 * @param filename YAML filename.
 */
void NewBattleState::save(const std::string &filename)
{
	YAML::Emitter out;
	YAML::Node node;
	node["mission"] = _cbxMission->getSelected();
	node["craft"] = _cbxCraft->getSelected();
	node["darkness"] = _slrDarkness->getValue();
	node["terrain"] = _cbxTerrain->getSelected();
	node["alienRace"] = _cbxAlienRace->getSelected();
	node["difficulty"] = _cbxDifficulty->getSelected();
	node["alienTech"] = _slrAlienTech->getValue();
	node["base"] = _game->getSavedGame()->getBases()->front()->save();
	out << node;

	std::string filepath = Options::getMasterUserFolder() + filename + ".cfg";
	if (!CrossPlatform::writeFile(filepath, out.c_str()))
	{
		Log(LOG_WARNING) << "Failed to save " << filepath;
		return;
	}
}

/**
 * Initializes a new savegame with
 * everything available.
 */
void NewBattleState::initSave()
{
	const Mod *mod = _game->getMod();
	SavedGame *save = new SavedGame();
	Base *base = new Base(mod);
	const YAML::Node &starter = _game->getMod()->getDefaultStartingBase();
	base->load(starter, save, true, true);
	save->getBases()->push_back(base);

	// Kill everything we don't want in this base
	for (std::vector<Soldier*>::iterator i = base->getSoldiers()->begin(); i != base->getSoldiers()->end(); ++i) delete (*i);
	base->getSoldiers()->clear();
	for (std::vector<Craft*>::iterator i = base->getCrafts()->begin(); i != base->getCrafts()->end(); ++i) delete (*i);
	base->getCrafts()->clear();
	base->getStorageItems()->getContents()->clear();

	_craft = new Craft(mod->getCraft(_crafts[_cbxCraft->getSelected()]), base, 1);
	base->getCrafts()->push_back(_craft);

	// Generate soldiers
	bool psiStrengthEval = (Options::psiStrengthEval && save->isResearched(mod->getPsiRequirements()));
	for (int i = 0; i < 30; ++i)
	{
		int randomType = RNG::generate(0, _game->getMod()->getSoldiersList().size() - 1);
		Soldier *soldier = mod->genSoldier(save, _game->getMod()->getSoldiersList().at(randomType));

		for (int n = 0; n < 5; ++n)
		{
			if (RNG::percent(70))
				continue;
			soldier->promoteRank();

			UnitStats* stats = soldier->getCurrentStats();
			stats->tu			+= RNG::generate(0, 5);
			stats->stamina		+= RNG::generate(0, 5);
			stats->health		+= RNG::generate(0, 5);
			stats->bravery		+= RNG::generate(0, 5);
			stats->reactions	+= RNG::generate(0, 5);
			stats->firing		+= RNG::generate(0, 5);
			stats->throwing		+= RNG::generate(0, 5);
			stats->strength		+= RNG::generate(0, 5);
			stats->mana			+= RNG::generate(0, 5);
			stats->psiStrength	+= RNG::generate(0, 5);
			stats->melee		+= RNG::generate(0, 5);
			stats->psiSkill		+= RNG::generate(0, 20);
		}
		UnitStats* stats = soldier->getCurrentStats();
		stats->bravery = (int)ceil(stats->bravery / 10.0) * 10; // keep it a multiple of 10

		// update again, could have been changed since soldier creation
		soldier->calcStatString(mod->getStatStrings(), psiStrengthEval);

		base->getSoldiers()->push_back(soldier);

		auto space = _craft->getSpaceAvailable();
		if (_craft->validateAddingSoldier(space, soldier))
		{
			soldier->setCraft(_craft);
		}
	}

	// Generate items
	const std::vector<std::string> &items = mod->getItemsList();
	for (std::vector<std::string>::const_iterator i = items.begin(); i != items.end(); ++i)
	{
		RuleItem *rule = _game->getMod()->getItem(*i);
		if (rule->getBattleType() != BT_CORPSE && rule->isRecoverable())
		{
			int howMany = rule->getBattleType() == BT_AMMO ? 2 : 1;
			base->getStorageItems()->addItem(*i, howMany);
			if (rule->getBattleType() != BT_NONE && rule->isInventoryItem())
			{
				_craft->getItems()->addItem(*i, howMany);
			}
		}
	}

	// Add research
	for (auto& pair : mod->getResearchMap())
	{
		save->addFinishedResearchSimple(pair.second);
	}

	_game->setSavedGame(save);
	save->setGamePtr(_game);
	cbxMissionChange(0);
}

/**
 * Starts the battle.
 * @param action Pointer to an action.
 */
void NewBattleState::btnOkClick(Action *)
{
	if (_craft)
	{
		// just in case somebody manually edited battle.cfg
		_craft->resetCustomDeployment();
	}
	save();
	if (_missionTypes[_cbxMission->getSelected()] != "STR_BASE_DEFENSE" && _craft->getNumTotalUnits() == 0)
	{
		return;
	}

	SavedBattleGame *bgame = new SavedBattleGame(_game->getMod(), _game->getLanguage());
	_game->getSavedGame()->setBattleGame(bgame);
	if (_game->getMod()->isFTAGame())
	{
		_game->getSavedGame()->setFtAGame(true);
	}
	bgame->setMissionType(_missionTypes[_cbxMission->getSelected()]);
	BattlescapeGenerator bgen = BattlescapeGenerator(_game);
	Base *base = 0;

	bgen.setTerrain(_game->getMod()->getTerrain(_terrainTypes[_cbxTerrain->getSelected()]));

	// base defense
	if (_missionTypes[_cbxMission->getSelected()] == "STR_BASE_DEFENSE")
	{
		base = _craft->getBase();
		bgen.setBase(base);
		_craft = 0;
	}
	// alien base
	else if (_game->getMod()->getDeployment(bgame->getMissionType())->isAlienBase())
	{
		AlienBase *b = new AlienBase(_game->getMod()->getDeployment(bgame->getMissionType()), -1);
		b->setId(1);
		b->setAlienRace(_alienRaces[_cbxAlienRace->getSelected()]);
		_craft->setDestination(b);
		bgen.setAlienBase(b);
		_game->getSavedGame()->getAlienBases()->push_back(b);
	}
	// ufo assault
	else if (_craft && _game->getMod()->getUfo(_missionTypes[_cbxMission->getSelected()]))
	{
		Ufo *u = new Ufo(_game->getMod()->getUfo(_missionTypes[_cbxMission->getSelected()]), 1);
		u->setId(1);
		_craft->setDestination(u);
		bgen.setUfo(u);
		// either ground assault or ufo crash
		if (RNG::generate(0,1) == 1)
		{
			u->setStatus(Ufo::LANDED);
			bgame->setMissionType("STR_UFO_GROUND_ASSAULT");
		}
		else
		{
			u->setStatus(Ufo::CRASHED);
			bgame->setMissionType("STR_UFO_CRASH_RECOVERY");
		}
		_game->getSavedGame()->getUfos()->push_back(u);
	}
	// mission site
	else
	{
		const AlienDeployment *deployment = _game->getMod()->getDeployment(bgame->getMissionType());
		const RuleAlienMission *mission = _game->getMod()->getAlienMission(_game->getMod()->getAlienMissionList().front()); // doesn't matter
		MissionSite *m = new MissionSite(mission, deployment, nullptr);
		m->setId(1);
		m->setAlienRace(_alienRaces[_cbxAlienRace->getSelected()]);
		_craft->setDestination(m);
		bgen.setMissionSite(m);
		_game->getSavedGame()->getMissionSites()->push_back(m);
	}

	if (_craft)
	{
		_craft->setSpeed(0);
		bgen.setCraft(_craft);
	}

	_game->getSavedGame()->setDifficulty((GameDifficulty)_cbxDifficulty->getSelected());

	bgen.setWorldShade(_slrDarkness->getValue());
	bgen.setAlienRace(_alienRaces[_cbxAlienRace->getSelected()]);
	bgen.setAlienItemlevel(_slrAlienTech->getValue());
	bgame->setDepth(_slrDepth->getValue());

	bgen.run();

	_game->popState();
	_game->popState();
	_game->pushState(new BriefingState(_craft, base));
	_craft = 0;
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void NewBattleState::btnCancelClick(Action *)
{
	if (!_surfaceBackup.empty())
	{
		cleanup();
		return;
	}

	save();
	_game->setSavedGame(0);
	_game->popState();
}

/**
 * Randomize the state
 * @param action Pointer to an action.
 */
void NewBattleState::btnRandomClick(Action *)
{
	initSave();

	_cbxMission->setSelected(RNG::generate(0, _missionTypes.size()-1));
	cbxMissionChange(0);
	_cbxCraft->setSelected(RNG::generate(0, _crafts.size() - 1));
	cbxCraftChange(0);
	_slrDarkness->setValue(RNG::generate(0, 15));
	_cbxTerrain->setSelected(RNG::generate(0, _terrainTypes.size() - 1));
	cbxTerrainChange(0);
	_cbxAlienRace->setSelected(RNG::generate(0, _alienRaces.size()-1));
	_cbxDifficulty->setSelected(RNG::generate(0, 4));
	_slrAlienTech->setValue(RNG::generate(0, _game->getMod()->getAlienItemLevels().size()-1));
}

/**
 * Shows the Craft Info screen.
 * @param action Pointer to an action.
 */
void NewBattleState::btnEquipClick(Action *)
{
	_game->pushState(new CraftInfoState(_game->getSavedGame()->getBases()->front(), 0));
}

/**
 * Updates Map Options based on the
 * current Mission type.
 * @param action Pointer to an action.
 */
void NewBattleState::cbxMissionChange(Action *)
{
	AlienDeployment *ruleDeploy = _game->getMod()->getDeployment(_missionTypes[_cbxMission->getSelected()]);
	std::set<std::string> terrains;

	// Get terrains associated with this mission
	std::vector<std::string> deployTerrains, globeTerrains;
	deployTerrains = ruleDeploy->getTerrains();
	if (deployTerrains.empty())
	{
		globeTerrains = _game->getMod()->getGlobe()->getTerrains("");
	}
	else
	{
		globeTerrains = _game->getMod()->getGlobe()->getTerrains(ruleDeploy->getType());
	}
	for (std::vector<std::string>::const_iterator i = deployTerrains.begin(); i != deployTerrains.end(); ++i)
	{
		terrains.insert(*i);
	}
	for (std::vector<std::string>::const_iterator i = globeTerrains.begin(); i != globeTerrains.end(); ++i)
	{
		terrains.insert(*i);
	}
	_terrainTypes.clear();
	std::vector<std::string> terrainStrings;
	for (std::set<std::string>::const_iterator i = terrains.begin(); i != terrains.end(); ++i)
	{
		_terrainTypes.push_back(*i);
		terrainStrings.push_back("MAP_" + *i);
	}

	// Hide controls that don't apply to mission
	_txtDarkness->setVisible(ruleDeploy->getShade() == -1);
	_slrDarkness->setVisible(ruleDeploy->getShade() == -1);
	_txtTerrain->setVisible(_terrainTypes.size() > 1);
	_cbxTerrain->setVisible(_terrainTypes.size() > 1);
	_cbxTerrain->setOptions(terrainStrings, true);
	_cbxTerrain->setSelected(0);
	_btnTerrain->setVisible(_missionTypes.size() > TFTD_DEPLOYMENTS && _terrainTypes.size() > 1);
	cbxTerrainChange(0);
}

/**
 * Updates craft accordingly.
 * @param action Pointer to an action.
 */
void NewBattleState::cbxCraftChange(Action *)
{
	_craft->changeRules(_game->getMod()->getCraft(_crafts[_cbxCraft->getSelected()]));

	int count = 0;
	Craft* tmpCraft = new Craft(_craft->getRules(), _craft->getBase(), 0);

	// temporarily re-assign all soldiers to a dummy craft
	for (auto* soldier : *_craft->getBase()->getSoldiers())
	{
		if (soldier->getCraft() == _craft)
		{
			soldier->setCraft(tmpCraft);
			count++;
		}
	}
	// try assigning all soldiers back while validating constraints
	for (auto* soldier : *_craft->getBase()->getSoldiers())
	{
		if (count <= 0)
		{
			break;
		}
		if (soldier->getCraft() == tmpCraft)
		{
			count--;
			auto space = _craft->getSpaceAvailable();
			if (_craft->validateAddingSoldier(space, soldier))
			{
				soldier->setCraft(_craft);
			}
			else
			{
				soldier->setCraft(0);
			}
		}
	}
	delete tmpCraft;

	// FIXME? HWPs can still violate the constraints (e.g. when switching from Avenger to Lightning)
}

/**
 * Updates the depth slider accordingly when terrain selection changes.
 * @param action Pointer to an action.
 */
void NewBattleState::cbxTerrainChange(Action *)
{
	AlienDeployment *ruleDeploy = _game->getMod()->getDeployment(_missionTypes[_cbxMission->getSelected()]);
	int minDepth = 0;
	int maxDepth = 0;
	if (ruleDeploy->getMaxDepth() > 0 || _game->getMod()->getTerrain(_terrainTypes.at(_cbxTerrain->getSelected()))->getMaxDepth() > 0 ||
		(!ruleDeploy->getTerrains().empty() && _game->getMod()->getTerrain(ruleDeploy->getTerrains().front())->getMaxDepth() > 0))
	{
		minDepth = 1;
		maxDepth = 3;
	}
	_txtDepth->setVisible(minDepth != maxDepth);
	_slrDepth->setVisible(minDepth != maxDepth);
	_slrDepth->setRange(minDepth, maxDepth);
	_slrDepth->setValue(minDepth);

	// Get races "supported" by this mission
	_alienRaces = _game->getMod()->getAlienRacesList();
	int maxAlienRank = ruleDeploy->getMaxAlienRank();
	for (std::vector<std::string>::iterator i = _alienRaces.begin(); i != _alienRaces.end();)
	{
		if ((*i).find("_UNDERWATER") != std::string::npos)
		{
			i = _alienRaces.erase(i);
		}
		else
		{
			std::string raceName = (minDepth != maxDepth) ? (*i) + "_UNDERWATER" : (*i);
			auto raceRules = _game->getMod()->getAlienRace(raceName);
			if (!raceRules || maxAlienRank >= raceRules->getMembers())
			{
				// not enough members or race doesn't exist
				i = _alienRaces.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
	_cbxAlienRace->setOptions(_alienRaces, true);
}

/**
 * Shows the advanced mission selector.
 */
void NewBattleState::btnMissionChange(Action *action)
{
	fillList(NewBattleSelectType::MISSION, _game->isRightClick(action));
}

/**
 * Shows the advanced terrain selector.
 */
void NewBattleState::btnTerrainChange(Action *action)
{
	fillList(NewBattleSelectType::TERRAIN, _game->isRightClick(action));
}

/**
 * Shows the advanced alien race selector.
 */
void NewBattleState::btnAlienRaceChange(Action *action)
{
	fillList(NewBattleSelectType::ALIENRACE, _game->isRightClick(action));
}

/**
 * Fills the advanced selector with data.
 */
void NewBattleState::fillList(NewBattleSelectType selectType, bool isRightClick)
{
	_selectType = selectType;
	_isRightClick = isRightClick;

	bool firstRun = false;
	if (_surfaceBackup.empty())
	{
		firstRun = true;
		for (auto surface : _surfaces)
		{
			_surfaceBackup[surface] = surface->getVisible();
			surface->setVisible(false);
		}
		_window->setVisible(true);
		_txtTitle->setVisible(true);
		_btnCancel->setVisible(true);
		_btnRandom->setVisible(false);
		_lstSelect->setVisible(true);
	}

	std::string searchString = _btnQuickSearch->getText();
	Unicode::upperCase(searchString);

	size_t counter = 0;
	auto fill = [&](const std::vector<std::string>& list, bool prefix, size_t scroll)
	{
		for (auto& m : list)
		{
			if (!searchString.empty())
			{
				std::string itemName;
				if (_isRightClick) { itemName = m; } else { itemName = tr(prefix ? "MAP_" + m : m); }
				Unicode::upperCase(itemName);
				if (itemName.find(searchString) == std::string::npos)
				{
					counter++;
					continue;
				}
			}
			_filtered.push_back(counter);
			counter++;
			_lstSelect->addRow(1, _isRightClick ? m.c_str() : tr(prefix ? "MAP_" + m : m).c_str());
		}
		if (firstRun && _lstSelect->isScrollbarVisible()) _lstSelect->scrollTo(scroll);
	};

	_filtered.clear();
	_lstSelect->clearList();
	if (_selectType == NewBattleSelectType::MISSION)
	{
		fill(_missionTypes, false, _cbxMission->getSelected());
	}
	else if (_selectType == NewBattleSelectType::TERRAIN)
	{
		fill(_terrainTypes, true, _cbxTerrain->getSelected());
	}
	else if (_selectType == NewBattleSelectType::ALIENRACE)
	{
		fill(_alienRaces, false, _cbxAlienRace->getSelected());
	}
}

/**
 * L-click: Selects an item from the advanced selector and closes the selector.
 * R-click: Displays the code of the selected item.
 * M-click: Displays the translation of the selected item.
 */
void NewBattleState::lstSelectClick(Action *action)
{
	auto selected = _lstSelect->getSelectedRow();

	// quick toggle
	if (_game->isRightClick(action) || _game->isMiddleClick(action))
	{
		auto& list = (_selectType == NewBattleSelectType::MISSION ? _missionTypes : (_selectType == NewBattleSelectType::TERRAIN ? _terrainTypes : _alienRaces));
		std::string s = list[_filtered[selected]];
		if (_game->isMiddleClick(action))
		{
			s = tr((_selectType == NewBattleSelectType::TERRAIN) ? "MAP_" + s : s);
		}
		_lstSelect->setCellText(selected, 0, s.c_str());
		return;
	}

	// restore the GUI
	cleanup();

	// select the new value
	if (_selectType == NewBattleSelectType::MISSION)
	{
		_cbxMission->setSelected(_filtered[selected]);
		cbxMissionChange(0);
	}
	else if (_selectType == NewBattleSelectType::TERRAIN)
	{
		_cbxTerrain->setSelected(_filtered[selected]);
		cbxTerrainChange(0);
	}
	else if (_selectType == NewBattleSelectType::ALIENRACE)
	{
		_cbxAlienRace->setSelected(_filtered[selected]);
	}
}

/**
 * Restores the GUI after closing the advanced selector.
 */
void NewBattleState::cleanup()
{
	_btnQuickSearch->setText("");
	_btnQuickSearch->setVisible(false);

	for (auto& surfacePair : _surfaceBackup)
	{
		surfacePair.first->setVisible(surfacePair.second);
	}
	_surfaceBackup.clear();
}

/**
 * Quick search toggle.
 * @param action Pointer to an action.
 */
void NewBattleState::btnQuickSearchToggle(Action *action)
{
	if (_surfaceBackup.empty())
	{
		return;
	}

	if (_btnQuickSearch->getVisible())
	{
		_btnQuickSearch->setText("");
		_btnQuickSearch->setVisible(false);
		btnQuickSearchApply(action);
	}
	else
	{
		_btnQuickSearch->setVisible(true);
		_btnQuickSearch->setFocus(true);
	}
}

/**
 * Quick search.
 * @param action Pointer to an action.
 */
void NewBattleState::btnQuickSearchApply(Action *)
{
	fillList(_selectType, _isRightClick);
}

}
